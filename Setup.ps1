$url = "https://github.com/yan0lovesha/HotkeyMaster/releases/latest/download/HotkeyMaster.exe"
$output = "$env:APPDATA\HotkeyMaster.exe"

# Kill the existing running application
Get-Process | Where-Object {$_.Path -eq $output} | Stop-Process

# Download the application
Invoke-WebRequest -Uri $url -OutFile $output

# Start the application immediately
& $output