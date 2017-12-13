#pragma once

#include <string>
#include <exception>
#include <vector>
#ifndef FILE_CONSTANTS
#define FILE_CONSTANTS
const std::string MODEL_BASEPATH_CONST = "ResourceFiles/Models/";
const std::string TEXTURE_BASEPATH_CONST = "ResourceFiles/Textures/";
const std::string MODEL_EXTENSION_CONST = ".obj";
const std::string TEXTURE_EXTENSION_CONST = ".png";
#endif

/// <summary>
/// Static helper class that wraps some SDL file operations
/// </summary>
class FileHelper {
public:

	/// <summary>
	/// Loads the contents of a file into a string
	/// </summary>
	/// <param name="path">String containing an absolute path to the file</param>
	/// <return>Contents of the file specified</return>
	/// <throws>FileNotFoundException if file is not found</throws>
	static std::string loadFileFromString(std::string path);

	/// <summary>
	/// Loads the contents of a file into a binary array
	/// </summary>
	/// <param name="path">String containing an absolute path to the file</param>
	/// <return>Contents of the file specified (binary)</return>
	/// <throws>FileNotFoundException if file is not found</throws>
	static std::vector<uint8_t> loadBinaryFileFromString(std::string path);

	/// <summary>
	/// Loads the contents of a file into an SDL_surface
	/// </summary>
	/// <param name="path">String containing an absolute path to the file</param>
	/// <return>Contents of the file specified (image as SDL_Surface pointer)</return>
	/// <throws>FileNotFoundException if file is not found</throws>
	/// <throws>FileImageConversionException if file cannot be converted to image</throws>
	static SDL_Surface* loadImageFileFromString(std::string path);

	/// <summary>
	/// Loads the contents of a file into a string (relative path)
	/// </summary>
	/// <param name="relativePath">String containing a path to the file relative to the application executable</param>
	/// <return>Contents of the file specified</return>
	/// <throws>FileNotFoundException if file is not found</throws>
	static std::string loadFileFromStringRelative(std::string relativePath);

	/// <summary>
	/// Loads the contents of a file into a binary array
	/// </summary>
	/// <param name="relativePath">String containing a path to the file relative to the application executable</param>
	/// <return>Contents of the file specified (binary)</return>
	/// <throws>FileNotFoundException if file is not found</throws>
	static std::vector<uint8_t> loadBinaryFileFromStringRelative(std::string relativePath);
	
	/// <summary>
	/// Loads the contents of a file into an SDL_surface
	/// </summary>
	/// <param name="path">String containing a relative path to the file</param>
	/// <return>Contents of the file specified (image as SDL_Surface pointer)</return>
	/// <throws>FileNotFoundException if file is not found</throws>
	/// <throws>FileImageConversionException if file cannot be converted to image</throws>
	static SDL_Surface* loadImageFileFromStringRelative(std::string relativePath);
};

/// <summary>
/// Exception thrown by FileHelper when it can't find a file
/// </summary>
class FileNotFoundException : public std::runtime_error {
public:
	FileNotFoundException(const std::string& what_arg) : std::runtime_error(what_arg) {}
};

/// <summary>
/// Exception thrown by FileHelper when it can't read an image file as an image
/// </summary>
class FileImageConversionException : public std::runtime_error {
public:
	FileImageConversionException(const std::string& what_arg) : std::runtime_error(what_arg) {}
};