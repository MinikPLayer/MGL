#include "Input.h"
#include "Debug.h"

vector<Input::Axis> Input::axisMappings;
Input::Axis Input::EmptyAxis = Input::Axis("EMPTY", Input::Keyboard, Input::NONE, Input::NONE);
Vector3 Input::lastMousePos;
Vector3 Input::mousePos;
Vector3 Input::mouseMove;

Input::Axis::Event::Event(GameObject* object, void(GameObject::* func)(Axis*))
{
    this->object = object;
    this->func = func;
}

void Input::Axis::Event::Exec(Axis* axis)
{
    if (object == nullptr || func == nullptr)
        return;

    // Exec function
    (object->*(func))(axis);
}

void Input::Axis::AddMapping(Mapping mapping)
{
    mappings.push_back(mapping);
}

Input::Axis::Mapping::Mapping(int device, int positiveKey, int negativeKey)
{
    this->device = device;
    this->positiveKey = positiveKey;
    this->negativeKey = negativeKey;
}

Input::Axis::Axis(string name, int device, int positiveKey, int negativeKey)
{
    //this->device = device;
    //this->name = name;
    //this->positiveKey = positiveKey;
    //this->negativeKey = negativeKey;
    this->name = name;
    
    AddMapping(Mapping(device, positiveKey, negativeKey));
}

Input::Axis Input::FindAxis(string name)
{
    for (int i = 0; i < axisMappings.size(); i++)
    {
        if (axisMappings[i].name == name)
            return axisMappings[i];
    }

    LOGW_E("Cannot find axis ", name);

    return EmptyAxis;
}

void Input::RegisterAxis(Axis axis)
{
    axisMappings.push_back(axis);
}

bool Input::GetButtonDown(string name)
{
    return FindAxis(name).GetDown();

    //return axis.lastValue == 0 && axis.value == 1;
}

bool Input::GetButton(string name)
{
    return FindAxis(name).Getvalue();//.value;
}

float Input::GetAxis(string name)
{
    Axis axis = FindAxis(name);

    return axis.Getvalue();//.value;
}

float Input::GetKeyValue(GLFWwindow* window, int device, int positiveKey, int negativeKey)
{
    float value = 0;
    switch (device)
    {
    case Input::Devices::Keyboard:
        value = (glfwGetKey(window, positiveKey) == GLFW_PRESS) - (glfwGetKey(window, negativeKey) == GLFW_PRESS);
        break;

    case Devices::Mouse:
        value = (positiveKey == MouseAxis::MOUSE_X) * mouseMove.x - // because y is inverted
            (positiveKey == MouseAxis::MOUSE_Y) * mouseMove.y -
            (negativeKey == MouseAxis::MOUSE_X) * mouseMove.x + // because y is inverted
            (negativeKey == MouseAxis::MOUSE_Y) * mouseMove.y;
        break;

    default:
        LOGW_E("Cannot find device with id ", device);
        break;
    }

    return value;
}

void Input::__Update(GLFWwindow* window)
{
    lastWindow = window;

    mouseMove = mousePos - lastMousePos;
    lastMousePos = mousePos;

    // Switch axis buffers
    for (int i = 0; i < axisMappings.size(); i++)
    {
        axisMappings[i].SwitchBuffers();
    }

    for (int i = 0; i < axisMappings.size(); i++)
    {
        /*axisMappings[i].lastValue = axisMappings[i].value;

        float value = 0;
        switch (axisMappings[i].device)
        {
        case Devices::Keyboard:     
             value = (glfwGetKey(window, axisMappings[i].positiveKey) == GLFW_PRESS) - (glfwGetKey(window, axisMappings[i].negativeKey) == GLFW_PRESS);
             break;

        case Devices::Mouse:
            value = (axisMappings[i].positiveKey == MouseAxis::MOUSE_X) * mouseMove.x - // because y is inverted
                (axisMappings[i].positiveKey == MouseAxis::MOUSE_Y) * mouseMove.y -
                (axisMappings[i].negativeKey == MouseAxis::MOUSE_X) * mouseMove.x + // because y is inverted
                (axisMappings[i].negativeKey == MouseAxis::MOUSE_Y) * mouseMove.y;
            break;

        default:
            LOGW_E("Cannot find device with id ", axisMappings[i].device);
            break;
        }

        axisMappings[i].value = value;*/

        for (int j = 0; j < axisMappings[i].mappings.size(); j++)
        {
            #define mapping axisMappings[i].mappings[j]

            float value = GetKeyValue(window, mapping.device, mapping.positiveKey, mapping.negativeKey);
            

            axisMappings[i].UpdateValue(value);

            #undef mapping
        }
        

        
    }
}



void Input::__SetMousePos(double xPos, double yPos)
{
    static bool firstMouse = true;

    if (firstMouse)
    {
        lastMousePos = Vector3(xPos, yPos);
        firstMouse = false;
    }


    mousePos = Vector3(xPos, yPos);
}

void Input::Axis::UpdateValue(float value)
{
    if (abs(value) > abs(this->value))
        this->value = value;
}

void Input::Axis::SwitchBuffers()
{
    this->lastValue = value;
    this->value = 0;
}

float Input::Axis::Getvalue()
{
    return value;
}

bool Input::Axis::GetDown()
{
    return lastValue == 0 && value == 1;
}

bool Input::GetKey(Devices device, int key)
{
    if (lastWindow == nullptr)
        return false;

    return GetKeyValue(lastWindow, device, key, KBButtons::NONE);
}

GLFWwindow* Input::lastWindow = nullptr;