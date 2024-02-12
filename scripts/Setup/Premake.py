import os
import sys
from pathlib import Path

import Utils

import colorama
from colorama import Fore
from colorama import Back
from colorama import Style

PREMAKE_VERSION = '5.0.0-beta2'
PREMAKE_PREBUILD_BINARIES_URL = f'https://github.com/premake/premake-core/releases/download/v{PREMAKE_VERSION}/premake-{PREMAKE_VERSION}-windows.zip'
PREMAKE_LISENCE_URL = f'https://raw.githubusercontent.com/premake/premake-core/master/LICENSE.txt'
PREMAKE_LOCAL_DIR = 'Tools/premake/bin'
PREMAKE_PATH = f'{PREMAKE_LOCAL_DIR}/premake-{PREMAKE_VERSION}-windows.zip'
PREMAKE_EXE_PATH = f'{PREMAKE_LOCAL_DIR}/premake5.exe'
PREMAKE_LICENSE_PATH = f'{PREMAKE_LOCAL_DIR}/LICENSE.txt'

colorama.init()

def InstallPremake():
    Path(PREMAKE_LOCAL_DIR).mkdir(parents=True, exist_ok=True)
    print('Downloading {} to {}'.format(PREMAKE_PREBUILD_BINARIES_URL, PREMAKE_EXE_PATH))
    Utils.DownloadFile(PREMAKE_PREBUILD_BINARIES_URL, PREMAKE_PATH)
    print(f"Extracting", PREMAKE_PATH)
    Utils.UnzipFile(PREMAKE_PATH)
    print("Downloading {} to {}".format(PREMAKE_LISENCE_URL, PREMAKE_LICENSE_PATH))
    Utils.DownloadFile(PREMAKE_LISENCE_URL, PREMAKE_LICENSE_PATH)

def InstallPremakePrompt():
    print("Would you like to install the Premake?")
    install = Utils.YesOrNo()
    if (install):
        InstallPremake()
        quit()

def CheckPremake():
    premakeExePath = Path(f"{PREMAKE_LOCAL_DIR}/premake5.exe")
    if (not premakeExePath.exists()):
        print(f"{Style.BRIGHT}{Back.RED}You don't have the Premake installed!{Style.RESET_ALL}")
        InstallPremakePrompt()
        return False
    print(f"{Style.BRIGHT}{Back.GREEN}Premake located at {PREMAKE_LOCAL_DIR}{Style.RESET_ALL}")
    return True