#r "nuget: System.IO.Abstractions, 12.0.0"
#r "isocd-builder.dll"

using System;
using System.IO;
using isocd_builder;

// --- Hardcoded ISO build settings ---

// Need to figure out how to include arguments or create an ini ?

var options = new Options
{
    InputFolder = "/media/iso",          // source files
    OutputFile = "/home/chris/iso/test.iso",             // ISO name
    VolumeId = "CDTVBOOT",               // ISO label
    TargetSystem = TargetSystemType.CDTV, // CDTV or CD32
    PadSize = PadSizeType.None,
    Trademark = true
};

// --- Step 1: Prepare trademark files if needed ---
Console.WriteLine("Initializing trademark setup...");
var setupResult = BuilderSetupHelper.Setup();

if (!setupResult.HaveTmFiles && options.Trademark)
{
    Console.WriteLine("Downloading trademark files...");
    var success = TmFileHelper.DownloadTmFiles();
    if (!success)
    {
        Console.WriteLine("Could not download TM files. Aborting.");
        return;
    }
    Console.WriteLine("TM files downloaded.");
}

// --- Step 2: Assign correct TM file based on TargetSystem ---
if (string.IsNullOrWhiteSpace(options.TrademarkFile) && options.Trademark)
{
    string tmFileName = options.TargetSystem == TargetSystemType.CDTV
        ? isocd_builder_constants.CDTV_TRADEMARK_FILE
        : isocd_builder_constants.CD32_TRADEMARK_FILE;

    options.TrademarkFile = Path.Combine(
        isocd_builder_constants.ISOCDWIN_PUBLIC_DOCUMENTS_PATH,
        tmFileName
    );

    Console.WriteLine($"Using default TM file: {options.TrademarkFile}");
}

// --- Step 3: Build the ISO ---
Console.WriteLine("Start ISO build...");
var worker = new BuildIsoWorker();

try
{
    worker.StartWork(options);
    Console.WriteLine("ISO build done!");
}
catch (Exception ex)
{
    Console.WriteLine("ISO build failed:");
    Console.WriteLine(ex.Message);
}

