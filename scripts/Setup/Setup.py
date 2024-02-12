import os
import subprocess
import CheckPython

# Make sure everything we need is installed
CheckPython.ValidatePackages()

import Premake
import Utils

import colorama
from colorama import Fore
from colorama import Back
from colorama import Style


# Change from Scripts directory to root
os.chdir('../')

colorama.init()

if (not Premake.CheckPremake()):
    print("Premake not installed.")
    exit()
    
print(f"{Style.BRIGHT}{Back.GREEN}Generating Visual Studio 2022 solution.{Style.RESET_ALL}")
subprocess.call(["Tools/premake/bin/premake5.exe", "vs2022"])
