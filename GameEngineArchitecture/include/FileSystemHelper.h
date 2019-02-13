/**
@file FileSystemHelper.h
@brief A class that aids getting files and file information from a folder.
*/
#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>

/**
	* A structure to represent file Information.
*/
struct FileInformation {
	std::string m_Name;	//!< The name of the file.
	std::string m_Extension;	//!< The file's extension.
	std::string m_Location;	//!< The file's location.
};

/*! \class FileSystemHelper
	\brief A class that aids getting files and file information from a folder.
*/
class FileSystemHelper {
private:
	FileSystemHelper() = default;	//!< Default constructor.
	~FileSystemHelper() = default;	//!< Default destructor.

	/**
		* A structure to represent a path leaf.
	*/
	struct PathLeafString {
		/*!
			\brief Reads in specific information about a file, from a folder.
			\return Returns the information it's read, about the file.
		*/
		FileInformation operator()(const std::filesystem::directory_entry &p_Entry) const {
			FileInformation fileInformation;

			// Add the Name of the file.
			fileInformation.m_Name = p_Entry.path().filename().string();
			RemoveFileExtension(fileInformation.m_Name);

			// Add the Extension of the file.
			fileInformation.m_Extension = p_Entry.path().extension().string();

			// Add the path of the file.
			fileInformation.m_Location = p_Entry.path().string();

			return fileInformation;
		}
	};

public:
	/*!
		\brief Gets all files in a folder.
		\param p_FolderLocation the folder location.
		\param p_SearchSubDirectories whether to search sub directories within the folder, or not.
		\return Returns the files found.
	*/
	static std::vector<FileInformation> GetFilesInFolder(const std::string &p_FolderLocation, bool p_SearchSubDirectories = true) {
		std::vector<FileInformation> files;

		struct stat info;
		if (stat(p_FolderLocation.c_str(), &info) != 0) {
			std::cerr << p_FolderLocation << " cannot be accessed." <<
				"\nMake sure it exists." << std::endl;

			return files;
		}

		if (!p_SearchSubDirectories) {
			std::filesystem::path pathLocation(p_FolderLocation);
			std::filesystem::directory_iterator start(pathLocation);
			std::filesystem::directory_iterator end;
			std::transform(start, end, std::back_inserter(files), PathLeafString());
		}
		else {
			std::filesystem::path pathLocation(p_FolderLocation);
			std::filesystem::recursive_directory_iterator rStart(pathLocation);
			std::filesystem::recursive_directory_iterator rEnd;
			std::transform(rStart, rEnd, std::back_inserter(files), PathLeafString());
		}

		return files;
	}

	/*!
		\brief Either keeps only certain file, with the given file extension[s], or it removes them, depending on what p_KeepFile is set to.
		\param p_Files the files to manipulate.
		\param p_FileExtensions the file extension[s] to either keep or remove.
		\param p_KeepFile whether to keep the particular file extensions passed in, or only remove files with those extensions - default value is to keep the files with the passed in extensions.
	*/
	static void RetainRemoveFilesWithExtensions(std::vector<std::string> &p_Files, const std::vector<std::string> &p_FileExtensions, bool p_KeepFile = true) {
		p_Files.erase(std::remove_if(p_Files.begin(), p_Files.end(), [&](std::string p_File)->bool {

			size_t extensionStartPosition = p_File.find_last_of(".");
			std::string fileExtension = p_File.substr(extensionStartPosition, p_File.size() - 1);
			for (auto &extension : p_FileExtensions) {
				if (fileExtension == extension && p_KeepFile)
					return false;
				if (fileExtension == extension && !p_KeepFile)
					return true;
			}

			return p_KeepFile;
		}), p_Files.end());
	}

	/*!
		\brief Either keeps only certain file, with the given file extension[s], or it removes them, depending on what p_KeepFile is set to.
		\param p_Files the files to manipulate.
		\param p_FileExtensions the file extension[s] to either keep or remove.
		\param p_KeepFile whether to keep the particular file extensions passed in, or only remove files with those extensions - default value is to keep the files with the passed in extensions.
	*/
	static void RetainRemoveFilesWithExtensions(std::vector<FileInformation> &p_Files, const std::vector<std::string> &p_FileExtensions, bool p_KeepFile = true) {
		p_Files.erase(std::remove_if(p_Files.begin(), p_Files.end(), [&](FileInformation p_File)->bool {
			for (auto &extension : p_FileExtensions) {
				if (p_File.m_Extension == extension && p_KeepFile)
					return false;
				if (p_File.m_Extension == extension && !p_KeepFile)
					return true;
			}

			return p_KeepFile;
		}), p_Files.end());
	}

	/*!
		\brief Removes the file extension from a file name string.
		\param p_FileName the file name to manipulate.
		\return Returns true if it was able to remove it, false otherwise.
	*/
	static bool RemoveFileExtension(std::string &p_FileName) {
		size_t firstPosition = p_FileName.find_last_of('.');

		if (firstPosition == std::string::npos)
			return false;

		p_FileName = p_FileName.substr(0, firstPosition);
		return true;
	}

	/*!
		\brief Removes the file path from a file string.
		\param p_FileName the file name to manipulate.
		\return Returns true if it was able to remove it, false otherwise.
	*/
	static bool RemoveFilePath(std::string &p_FileName) {
		size_t firstPosition = p_FileName.find_last_of('/');

		if (firstPosition == std::string::npos)
			return false;

		p_FileName = p_FileName.substr(firstPosition + 1, p_FileName.size() - firstPosition);
		return true;
	}

	// Delete the copy and assignment operators.
	FileSystemHelper(FileSystemHelper const&) = delete; //!< Copy operator, deleted.
	FileSystemHelper& operator=(FileSystemHelper const&) = delete; //!< Assignment operator, deleted.
};