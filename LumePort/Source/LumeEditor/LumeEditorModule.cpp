#include "LumeEditorModule.h"
#include "Modules/ModuleManager.h"
#include "GameModes/LumeExperienceManager.h"

IMPLEMENT_MODULE(FLumeEditorModule, LumeEditor);

DEFINE_LOG_CATEGORY(LogLumeEditor);

void FLumeEditorModule::StartupModule()
{
	if (!IsRunningGame())
	{
		FEditorDelegates::BeginPIE.AddRaw(this, &ThisClass::OnBeginPIE);
		FEditorDelegates::EndPIE.AddRaw(this, &ThisClass::OnEndPIE);
	}
}

void FLumeEditorModule::ShutdownModule()
{
	FEditorDelegates::BeginPIE.RemoveAll(this);
	FEditorDelegates::EndPIE.RemoveAll(this);
}

void FLumeEditorModule::OnBeginPIE(bool bIsSimulating)
{
	ULumeExperienceManager* ExperienceManager = GEngine->GetEngineSubsystem<ULumeExperienceManager>();
	check(ExperienceManager);
	ExperienceManager->OnPlayInEditorBegun();
}

void FLumeEditorModule::OnEndPIE(bool bIsSimulating)
{
}