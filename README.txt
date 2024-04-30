INSTALLATION INSTRUCTIONS:
READ ALL ASSOCIATED WARNINGS BEFORE STARTING A STEP

Download the InstallWork.zip file from: https://www.mediafire.com/file/k3z2h4jgo30a0y6/InstallWork.zip/file 
Unzip the "InstallWork.zip" file. In the unzipped file, follow these instructions to install all dependencies:

1. Install "CMake" using the installer in folder "CMake_installer".
	
	a. WARNING: Select "Add CMake to system PATH for all users" when options appear. This selection is essential.

2. Install GEANT4 using the installer in folder "Geant4_installer".
	
	a. WARNING: DURING INSTALLATION, SET THE DIRECTORY OF INSTALLATION
		    TO THE FOLDER "VisualGEANT4".

	b. WARNING: Select "Add geant4 to the system PATH for all users" when options appear. This selection is essential.

3. Install Visual Studio Build Tools (Including MSBuild) using the installer in folder "MSBuild_installer."
	(this might take time as download is 8GB)	

	a. WARNING: Select the "C++ tools" option checkbox when shown.
	
	b. WARNING: For those with time constraints, Step 3. is optional. VisualGEANT4 will still function properly (except, the "recompile project" button will not work)
	
	c. WARNING: DURNING INSTALLATION, KEEP THE DEFAULT DIRECTORY OF INSTALLATION IF POSSIBLE
		    Default Directory: C:/"Program Files (x86)"/"Microsoft Visual Studio"/2022/BuildTools/MSBuild/Current/Bin
		    (If you installed MSBuild in a different directory, find the file MSBuildDIR.txt and change its content to this new directory. Use '/' instead of '\' in directories, and surround folders with whitespaces with quotation marks, as in above default directory.)

4. Download the required datasets (datasets.rar) from: https://www.mediafire.com/file/baqbtd4s41xs1kq/datasets.rar/file
	(this may take time, as there are ~2GB of data files.)

5. Extract/Unzip the folder in "datasets.rar" to any directory, though extracting inside the VisualGEANT4 file is preferred.

6. Click on the extracted file (geant4-data) and double click the batch file "geant4.bat" to run it.
	(a command line should appear)

7. Installation is complete! Now run the product under VisualGEANT4/VisualGEANT4.exe 
	(create a Desktop shortcut to VisualGEANT4/VisualGEANT4.exe if you prefer)
