/*#pragma once

#include <string>
#include <vector>
#include "Util.h"
using namespace std;

template<class T>
class Asset
{
	static vector<Asset*> assets;

	T* handler = nullptr;

	/// <summary>
	/// References numnber to this asset, uses to keep track if need deleting
	/// </summary>
	int refCount = 0;

	/// <summary>
	/// If this is not an original asset, this pointer would point to the original
	/// </summary>
	Asset* reference = nullptr;

	int& GetRefCounter()
	{
		if (reference == nullptr)
			return refCount;
		else
			return reference->refCount;
	}
public:
	/// <summary>
	/// Path to asset
	/// </summary>
	string path = "";


	Asset() {}

	Asset(const Asset& asset)
	{
		this->path = path;
		Init(asset.reference);
	}

	Asset(string path, bool skipInit = false)
	{
		LOGW("Asset constructor: \"", path, "\"");
		Init(path, nullptr, skipInit);
	}

	Asset(string path, T* instance, bool skipInit = false)
	{
		Init(path, instance, skipInit);
	}

	void Init(Asset* asset)
	{
		reference = asset;
		GetRefCounter()++; // increase original asset reference counter
	}

	void Init(string path, T* instance = nullptr, bool skipInit = false)
	{
		this->path = path;


		if (skipInit)
		{
			if (instance == nullptr)
				handler = new T();
			else
				handler = instance;
			refCount = 1;
		}
		else
		{
			bool found = false;
			// Add to assets
			if (path != "")	// if path is empty, we don't want to add it to assets list
			{
				for (int i = 0; i < assets.size(); i++)
				{
					if (assets[i]->path == path)
					{
						//reference = assets[i];
						//GetRefCounter()++; // increase original asset reference counter
						Init(assets[i]);
						found = true;
						break;
					}
				}
			}

			if (!found)
			{
				reference = new Asset(path, instance, true);
				assets.push_back(reference);
			}
		}
	}

	T* Get()
	{
		if (reference == nullptr)
			return handler;
		else
			return reference->Get();
	}

	void Dispose()
	{
		if (handler != nullptr)
		{
			delete handler;
			handler = nullptr;
		}

		bool found = false;
		for (int i = 0; i < assets.size(); i++)
		{
			if (assets[i] == this)
			{
				found = true;
				assets.erase(assets.begin() + i, assets.begin() + i + 1);
				break;
			}
		}

		if (!found)
			LOGE_E("Trying to dispose not registered asset, probably something went wrong");
	}

	~Asset() 
	{
		LOGW("Asset destructor");
		int ref = --GetRefCounter();
		if (ref == 0)
		{
			//Dispose();
			if (reference != nullptr)
			{
				reference->Dispose();
				delete reference;
				reference = nullptr;
			}
		}
	}
};


template<class T>
vector<Asset<T>*> Asset<T>::assets;*/