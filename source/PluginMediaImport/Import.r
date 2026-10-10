type 'IMPT'
{
	longint;
};

resource 'IMPT' (1000)
{
	// drawtype - this unique fourcc is required by After Effects,
	// and is not related to the filetype(s) supported by this importer
	FILE_IMPORT_FOUR_CC
};
