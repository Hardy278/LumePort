#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Logging/LogMacros.h"

DECLARE_LOG_CATEGORY_EXTERN(LogLumeEditor, Log, All);

class FLumeEditorModule : public FDefaultModuleImpl
{
	typedef FLumeEditorModule ThisClass;

public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void OnBeginPIE(bool bIsSimulating);
	void OnEndPIE(bool bIsSimulating);
};
