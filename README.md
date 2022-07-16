# Nuke
Nuke is a C++ library that borrows from [dotnet](https://github.com/dotnet) design.
It is primarily focused on ease of use rather than performance.

# Current Status
<font color=#FF0000 >It is not completed yet. We have a lot of work to do.</font>

# Base dependencies
## C++20
This framework is under C++ 20 standard
## GTest
Using Google `GTest` framework for the Nuke framework Testing


# Quick Start In Windows
## Windows 10 + SSH + WSL + Microsoft Visual Studio 2022 + Clang
1. Confirm already installed the vs 2022 with clang support and linux C++ support
2. Confirm WSL feature has been opened in windows 10 , if NOT, please open WSL for linux Clang debug environment. 
3. Download Ubuntu 22.04 LTS(recommended) from Microsoft Store
4. Install SSH client and server  both WSL Ubuntu && Windows  
- Windows  
* Open Powershell as ADMIN  
```powershell
# Install SSH
Get-WindowsCapability -Online | ? Name -like 'OpenSSH*'
Add-WindowsCapability -Online -Name OpenSSH.Client~~~~0.0.1.0
Add-WindowsCapability -Online -Name OpenSSH.Server~~~~0.0.1.0

# Configure ssh
Start-Service sshd
# OPTIONAL but recommended:
Set-Service -Name sshd -StartupType 'Automatic'
# Confirm the Firewall rule is configured. It should be created automatically by setup. 
Get-NetFirewallRule -Name *ssh*
# There should be a firewall rule named "OpenSSH-Server-In-TCP", which should be enabled
# If the firewall does not exist, create one
New-NetFirewallRule -Name sshd -DisplayName 'OpenSSH Server (sshd)' -Enabled True -Direction Inbound -Protocol TCP -Action Allow -LocalPort 22
```
- Ubuntu  
* Install openssh  
``sudo apt install openssh-client``  
``sudo apt install openssh-server``
* Start ssh service  
``ssh-keygen -A``   
``sudo /etc/init.d/ssh start``
* Edit ssh configuration  
``vi /etc/ssh/sshd_config``  
```
PermitRootLogin yes
PasswordAuthentication yes
``` 
* Restart ssh service  
``ssh-keygen -A``  
``service ssh restart``  

5. Install Cmake through ``apt install cmake``
6. Install Ninja as build system, ``apt update -y``  and ``apt install -y ninja-build``  
7. Install Clang as the compiler ``apt install clang``   
8. Install GTest ``apt install libgtest-dev``