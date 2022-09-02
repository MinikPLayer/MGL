#include "UserScript.h"

shared_ptr<GameObject> UserScript::AddCube(Vector3 pos)
{
	static shared_ptr<Model> cubeModel;
	if (cubeModel.get() == nullptr)
		cubeModel = AssetsLoader::LoadModel("cube_uv.fbx", mats[0]);
	//model = AssetsLoader::LoadModel("teapot1_01.FBX");
	//model = AssetsLoader::LoadModel("room.fbx", mats[0]);

	auto spawnedCube = cubeModel->SpawnMesh();
	spawnedCube->SetParent(this);
	spawnedCube->SetPosition(Vector3(0, 0, -10));
	spawnedCube->SetScale(Vector3(1, 1, 1));
	spawnedCube->SetRotation(Vector3(0, 0, 90));

	cubesSpawned++;

	return spawnedCube;
}

shared_ptr<GameObject> UserScript::AddTeapot(Vector3 pos)
{
	if (model.get() == nullptr)
		model = AssetsLoader::LoadModel("teapot1_01.FBX", mats[0]);
	//model = AssetsLoader::LoadModel("teapot1_01.FBX");
	//model = AssetsLoader::LoadModel("room.fbx", mats[0]);

	spawned = model->SpawnMesh();
	spawned->SetParent(this);
	spawned->SetPosition(Vector3(0, 0, -10));
	spawned->SetScale(Vector3(1, 1, 1));
	spawned->SetRotation(Vector3(-90, 0, 0));

	//cubesSpawned++;

	return spawned;
}


void UserScript::AddRandomCube()
{
	const int mult = 100;
	Vector3 dir = Vector3((rand() % (max * mult)) / mult - max / 2, (rand() % (max * mult)) / mult - max / 2, (rand() % (max * mult)) / mult - max / 2);
	dir.Normalize();

	AddCube(dir * ((rand() % (radius * mult)) / mult) /* distance */);
}

void UserScript::Start()
{

	srand(time(NULL));
	LOG("UserScript start");

	// Add light cube
	for (int i = 0; i < lightCubesCount; i++) {
		LightCube* cube = new LightCube();
		cube->SetPosition(Vector3(i, i + 1, i));
		lightCubes[i] = AddComponent(cube);
	}

	mats[0] = Material::CreatePrefabedMaterial(Material::SILVER);
	mats[1] = Material::CreatePrefabedMaterial(Material::RUBBER);
	mats[2] = Material::CreatePrefabedMaterial(Material::SILVER);
	mats[3] = Material::CreatePrefabedMaterial(Material::SILVER);
	mats[4] = Material::CreatePrefabedMaterial(Material::SILVER);

	LightCube* plane = new LightCube();
	plane->SetMaterial(mats[2]);
	plane->SetScale(Vector3(1.1, 1.2, 1.3f));
	plane->SetPosition(Vector3(-5, 5, -5));
	AddComponent(plane);


	dirLight = shared_ptr<DirectionalLight>(new DirectionalLight());
	dirLight->SetPosition(Vector3(20.f, 10.f, 30.f));
	dirLight->AddComponent(new Cube());

	Camera::GetMainCamera()->SetPosition(dirLight->GetPosition());
	Camera::GetMainCamera()->LookAt(Vector3(0, 0, 0));

	// Load textures
	for (int i = 0; i < 3; i++)
	{
		mats[i]->SetVec3("dirLight.direction", [this]() {
			return -1 * dirLight->GetPosition();
        });
		mats[i]->SetVec3("dirLight.ambient", Vector3(0.05f, 0.05f, 0.05f));
		mats[i]->SetVec3("dirLight.diffuse", Vector3(0.4f, 0.4f, 0.4f));
		mats[i]->SetVec3("dirLight.specular", Vector3(0.5f, 0.5f, 0.5f));
		mats[i]->SetFloat("dirLight.strength", 1.0f);

		mats[i]->SetVec3("viewPos", []() {
			return Camera::GetMainCamera()->GetPosition();
		});

		// TODO: Add engine wise lightning and shadowmaps
		auto shadowMap = dirLight->shadowMap;
		auto txt = shared_ptr<Texture>(new Texture(shadowMap->GetDepthMapID(), 15));
		mats[i]->SetTextureSlot(txt);
		mats[i]->SetBool("dirLight.castShadow", true);
		mats[i]->SetMat4("lightSpaceMatrix", [this]() {
			auto sm = dirLight->shadowMap;
            if(sm != nullptr)
			    return sm->lightSpaceMatrix;
            else
                return glm::mat4(1);
        });
		mats[i]->SetInt("shadowMap", 15);
	}



	//mats[0].get()->SetMaterialTexture(AssetsLoader::LoadTexture("poland.png"));
	//mats[0]->SetMaterialTexture(AssetsLoader::LoadTexture("cube_texture.png"));
	mats[0]->SetMaterialTexture(AssetsLoader::LoadTexture("TeaPot_low_map_A_teapot1_BaseColor.jpg"));
	//mats[0]->SetMaterialTexture(AssetsLoader::LoadTexture("TeaPot_low_map_A_teapot1_Metallic.jpg", 1), "specular");
	mats[0]->SetMaterialTexture(AssetsLoader::LoadTexture("TeaPot_low_map_A_teapot1_Metallic.jpg", 1), "specular");
	mats[0]->SetMaterialTexture(AssetsLoader::LoadTexture("TeaPot_low_map_A_teapot1_Normal.jpg", 2), "normal");

	//mats[2]->SetMaterialTexture(AssetsLoader::LoadTexture("grass.png"));
	//mats[2]->SetMaterialTexture(AssetsLoader::LoadTexture("grass_specular.png", 1), "specular");

	mats[2]->SetMaterialTexture(AssetsLoader::LoadTexture(Path::Combine({"ForestGrass", "_diffuse.tga"}), 0));
	mats[2]->SetMaterialTexture(AssetsLoader::LoadTexture(Path::Combine({"ForestGrass", "_occlusion.tga"}), 1), "specular");
	mats[2]->SetMaterialTexture(AssetsLoader::LoadTexture(Path::Combine({"ForestGrass", "_normals.tga"}), 2), "normal");

	
	auto forestCube = AddCube();
	Mesh* forestCubeMesh = (Mesh*)(forestCube->GetComponents()[0].get());

	mats[2]->SetVec2("texScale", Vector2(10, 10));
	forestCubeMesh->SetScale(Vector3(1, 25, 25));
	forestCubeMesh->SetMaterial(mats[2]);
	forestCubeMesh->SetPosition(Vector3(-30, 10, -30));

	//mats[3]->SetMaterialTexture(AssetsLoader::LoadTexture("tall_grass.png"));
	//mats[3]->SetMaterialTexture(AssetsLoader::LoadTexture("grass_specular.png", 1), "specular");

	//mats[4]->SetMaterialTexture(AssetsLoader::LoadTexture("rock1.png"));
	//mats[4]->SetMaterialTexture(AssetsLoader::LoadTexture("rock1_specular.png", 1), "specular");
	//mats[4]->SetMaterialTexture(AssetsLoader::LoadTexture("TeaPot_low_map_A_teapot1_BaseColor.jpg"));

	auto planeModel = AssetsLoader::LoadModel("plane2.fbx", mats[2]);
	auto planeMesh = planeModel->SpawnMesh();
	planeMesh->SetParent(this);
	planeMesh->SetRotation(Vector3(0, 0, 0));
	planeMesh->SetScale(Vector3(1, 1, 1));
	planeMesh->SetPosition(Vector3(0, 5, 0));

	// Add Floor
	floorMesh = shared_ptr<Mesh>(new Mesh(mats[2]));
	Vector2 sinMeshSize(100, 100);
	floorMesh->SetLocalPosition(-1.0f * Vector3(sinMeshSize.x / 2, 0, sinMeshSize.y / 2));
	floorMesh->GenerateMesh(sinMeshSize, [](float x, float y) {
		//return 0.5f * sin(x/4.f) * sin(y/4.f);
		return 0.5f * sin(x / 4.f) * sin(y / 2.f) + sin(x/3.f);
		//return 0.0f;
    }, nullptr, 1);
	AddComponent(floorMesh);

	LOG("Legend | PlaneMesh | GeneratedMesh");
	Mesh* planeMeshMesh = (Mesh*)planeMesh->GetComponents()[0].get();
	LOG("VertexCount | ", planeMeshMesh->vertexDataSize, " | ", floorMesh->vertexDataSize);

	// auto secondFloorMesh = shared_ptr<Mesh>(new Mesh(mats[2]));
	// secondFloorMesh->SetLocalPosition(Vector3(10, 5, 10));
	// secondFloorMesh->SetScale(Vector3(50, 1, 50));
	// secondFloorMesh->GenerateMesh(Vector2(1, 1), [](float x, float y) {
	// 	return 0.0f;
	// }, nullptr, 0.5);
	// AddComponent(secondFloorMesh);

	// Transparent plant
	/*PlaneMesh* plane = new PlaneMesh();
	plane->SetMaterial(mats[3]);
	plane->SetScale(Vector3(5, 5, 5));
	plane->SetPosition(Vector3(5, 10, 5));
	plane->faceCulling = Mesh::FaceCullingModes::Disabled;*/
	//Cube* plane = new Cube();


	AddTeapot();

	startPos = GetPosition();

	LOG("UserScript Components size: ", (int)GetComponents().size());

	Input::RegisterAxis(Input::Axis("AddCube", Input::Keyboard, Input::NUM_ADD));
	Input::RegisterAxis(Input::Axis("BindCamera", Input::Keyboard, Input::B));
	Input::RegisterAxis(Input::Axis("Pause", Input::Keyboard, Input::P));

	Input::RegisterAxis(Input::Axis("LightX", Input::Devices::Keyboard, Input::KBButtons::RIGHT, Input::KBButtons::LEFT));
	Input::RegisterAxis(Input::Axis("LightZ", Input::Devices::Keyboard, Input::KBButtons::UP, Input::KBButtons::DOWN));
	Input::RegisterAxis(Input::Axis("LightY", Input::Devices::Keyboard, Input::KBButtons::PGUP, Input::KBButtons::PGDOWN));

	Input::RegisterAxis(Input::Axis("RotationX", Input::Devices::Keyboard, Input::KBButtons::RIGHT_BRACKET, Input::KBButtons::LEFT_BRACKET));
	Input::RegisterAxis(Input::Axis("RotationY", Input::Devices::Keyboard, Input::KBButtons::APOSTROPHE, Input::KBButtons::SEMICOLON));
	Input::RegisterAxis(Input::Axis("RotationZ", Input::Devices::Keyboard, Input::KBButtons::SLASH, Input::KBButtons::PERIOD));
}

void UserScript::Update()
{
	float radius = 40;
	Vector3 dlpos = Vector3(radius * sin(Time::elapsedTime), 2, radius * cos(Time::elapsedTime));
	//dirLight->SetPosition(dlpos);

	for (int i = 0; i < lightCubesCount; i++) {
		LightCube* lCube = (LightCube*)lightCubes[i].get();

		const float lightMoveSpeed = 10;
		Vector3 move(0, 0, 0);
		move.x += Input::GetAxis("LightX") * Time::deltaTime * lightMoveSpeed;
		move.y += Input::GetAxis("LightY") * Time::deltaTime * lightMoveSpeed;
		move.z += -Input::GetAxis("LightZ") * Time::deltaTime * lightMoveSpeed;

		lCube->Move(move * (i + 1));
	}

	if (Input::GetButtonDown("Pause"))
	{
		paused = !paused;
		LOG("Paused: ", paused);
	}

	// Crash button
	if(Input::GetKey(Input::Keyboard, Input::KBButtons::T)) {
		if(Input::GetKey(Input::Keyboard, Input::KBButtons::I)) {
			mats[2]->SetMaterialTexture(AssetsLoader::LoadTexture("grass_specular_inverted.png", 1), "specular");
		}
		else if(Input::GetKey(Input::Keyboard, Input::KBButtons::N)) {
			mats[2]->SetMaterialTexture(AssetsLoader::LoadTexture("grass_specular.png", 1), "specular");
		}
	}

	if (Input::GetButtonDown("BindCamera"))
	{
		LOG("Bind camera");
		GameObject* parent = Camera::GetMainCamera()->GetParent();
		if (parent == nullptr)
			Camera::GetMainCamera()->SetParent(mesh.get());
		else
			Camera::GetMainCamera()->SetParent(nullptr);
	}

	float val = sin(Time::elapsedTime);
	if (Input::GetKey(Input::Keyboard, Input::KBButtons::C))
	{
		LOG("Val: ", val);
	}

	if (Input::GetKey(Input::Keyboard, Input::KBButtons::LCTRL)) {
		glDisable(GL_FRAMEBUFFER_SRGB);
		Window::GetMainWindow()->SetCursorMode(Window::CursorModes::Enabled);
	}
	else {
		glEnable(GL_FRAMEBUFFER_SRGB);
		Window::GetMainWindow()->SetCursorMode(Window::CursorModes::Disabled);
	}

	if (!paused)
		floorMesh->Rotate(Vector3(0, Time::deltaTime, 0));

	//auto s = abs(sin(Time::elapsedTime));
	//lightCubes[0]->SetScale(Vector3(s,s,s));

	//spawned->SetScale(Vector3(val, val, val));
	//lightCube->SetScale(Vector3(val, val, val));
	//this->SetScale(Vector3(val, val, val));
	//this->SetRotation(Vector3(val, val, val));
	//spawned->Rotate(Vector3(Input::GetAxis("RotationX"), Input::GetAxis("RotationY"), Input::GetAxis("RotationZ")) * Time::deltaTime);
	//mats[4]->SetVec2("texScale", Vector2(sin(Time::elapsedTime), cos(Time::elapsedTime)));
	//spawned->SetScale(Vector3(0.00001f, 0.00001f, 0.00001f));
}