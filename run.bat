Debug\XMLMetadataManager -r . *.cpp *.h

Debug\XMLMetadataManager -s main

Debug\XMLMetadataManager -m Package TestingPackage1 TestingPackage2

@echo "Checking if the modified dependency files come up in scan"

Debug\XMLMetadataManager -s main