$url = "https://github.com/yan0lovesha/HotkeyMaster/releases/latest/download/HotkeyMaster.exe"
$output = "$env:APPDATA\HotkeyMaster.exe"

# Kill the existing running application
Get-Process | Where-Object {$_.Path -eq $output} | Stop-Process

# Download the application
Invoke-WebRequest -Uri $url -OutFile $output

# Add the application to run at startup
$Startup = "HKCU:\SOFTWARE\Microsoft\Windows\CurrentVersion\Run"
Set-ItemProperty -Path $Startup -Name "MyApplication" -Value $output

# Start the application immediately
& $output