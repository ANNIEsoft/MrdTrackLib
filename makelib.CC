{
TString compilerflags = "-fdiagnostics-color=always -Wno-sign-compare";
gSystem->SetFlagsOpt(compilerflags.Data());
gSystem->SetFlagsDebug(compilerflags.Data());
TString pwd = gSystem->Getenv("PWD");

gInterpreter->AddIncludePath(pwd);
gSystem->AddIncludePath(TString::Format("%s%s","-I",pwd.Data()));

TString callcommand = ".L "+pwd+"/libMakeMrdTrackClasses.cxx++g";
gROOT->ProcessLine(callcommand.Data());
}
