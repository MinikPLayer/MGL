#pragma once

#include <string>
#include <vector>
#include <memory>

using namespace std;

class Asset
{
	// Type hash, used to check if object is a certain type
	size_t typeHash = 0;
public:
	/// <summary>
	/// Checks if this asset is of specified type
	/// </summary>
	/// <typeparam name="Type">Type to compare</typeparam>
	/// <returns>True if object is an instance of specified type, false otherwise</returns>
	template<class Type>
	bool IsAssetType()
	{
		return typeid(Type).hash_code() == typeHash;
	}

	/// <summary>
	/// [ENGINE] Sets asset type
	/// </summary>
	/// <typeparam name="Type">Object type</typeparam>
	template<class Type>
	void __SetAssetType()
	{
		typeHash = typeid(Type).hash_code();
	}

	string path;

	Asset(string path = "") { this->path = path; }
};