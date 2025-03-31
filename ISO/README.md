# CDTV compatible ISO creation for Linux. #



1. Install dotnet for arm64 https://learn.microsoft.com/en-us/dotnet/iot/deployment
2. Install dotnet scripting  dotnet tool install -g dotnet-script
3. Add to the path echo 'export PATH="$PATH:$HOME/.dotnet/tools"' >> ~/.bashrc  & source ~/.bashrc
4. Check it works dotnet-script --version
5. Grab the ISOCD-win release https://github.com/fuseoppl/isocd-win/releases
6. Copy the isocd-builder.dll to the Pi  e.g  /home/chris/isocd/
7. Edit options in buildiso.csx
8. Run the csx dotnet-script buildiso.csx

## Attribution ## 

Thanks to Ben Squibb (Stat-Mat) for the original ISOCD-Win and Meciek S (fuseoppl) for the forked version the DLL is taken from. 
Thanks to chriskuta for getting this working. 
