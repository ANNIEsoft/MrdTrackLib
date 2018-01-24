{
TString compilerflags = "-fdiagnostics-color=always -Wno-sign-compare";
TString pwd = gSystem->Getenv("PWD");

gInterpreter->AddIncludePath(pwd);
gSystem->AddIncludePath(TString::Format("%s%s","-I",pwd.Data()));

TString callcommand = ".L "+pwd+"/MakeMrdTrackClasses.cxx++g";
gROOT->ProcessLine(callcommand.Data());
}
