/*
  VitaShell
  Copyright (C) 2015-2018, TheFloW

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "main.h"
#include "browser.h"
#include "init.h"
#include "file.h"
#include "package_installer.h"
#include "utils.h"
#include "qr.h"
#include "rif.h"

#include "audio/vita_audio.h"

INCLUDE_EXTERN_RESOURCE(english_us_txt);

INCLUDE_EXTERN_RESOURCE(theme_txt);

INCLUDE_EXTERN_RESOURCE(default_colors_txt);
INCLUDE_EXTERN_RESOURCE(default_archive_icon_png);
INCLUDE_EXTERN_RESOURCE(default_audio_icon_png);
INCLUDE_EXTERN_RESOURCE(default_battery_bar_charge_png);
INCLUDE_EXTERN_RESOURCE(default_battery_bar_green_png);
INCLUDE_EXTERN_RESOURCE(default_battery_bar_red_png);
INCLUDE_EXTERN_RESOURCE(default_battery_png);
INCLUDE_EXTERN_RESOURCE(default_cover_png);
INCLUDE_EXTERN_RESOURCE(default_fastforward_png);
INCLUDE_EXTERN_RESOURCE(default_fastrewind_png);
INCLUDE_EXTERN_RESOURCE(default_file_icon_png);
INCLUDE_EXTERN_RESOURCE(default_folder_icon_png);
INCLUDE_EXTERN_RESOURCE(default_ftp_png);
INCLUDE_EXTERN_RESOURCE(default_image_icon_png);
INCLUDE_EXTERN_RESOURCE(default_pause_png);
INCLUDE_EXTERN_RESOURCE(default_play_png);
INCLUDE_EXTERN_RESOURCE(default_sfo_icon_png);
INCLUDE_EXTERN_RESOURCE(default_text_icon_png);
INCLUDE_EXTERN_RESOURCE(default_file_symlink_icon_png);
INCLUDE_EXTERN_RESOURCE(default_folder_symlink_icon_png);

INCLUDE_EXTERN_RESOURCE(electron_colors_txt);
INCLUDE_EXTERN_RESOURCE(electron_archive_icon_png);
INCLUDE_EXTERN_RESOURCE(electron_audio_icon_png);
INCLUDE_EXTERN_RESOURCE(electron_battery_bar_charge_png);
INCLUDE_EXTERN_RESOURCE(electron_battery_bar_green_png);
INCLUDE_EXTERN_RESOURCE(electron_battery_bar_red_png);
INCLUDE_EXTERN_RESOURCE(electron_battery_png);
INCLUDE_EXTERN_RESOURCE(electron_bg_audioplayer_png);
INCLUDE_EXTERN_RESOURCE(electron_bg_browser_png);
INCLUDE_EXTERN_RESOURCE(electron_bg_hexeditor_png);
INCLUDE_EXTERN_RESOURCE(electron_bg_photoviewer_png);
INCLUDE_EXTERN_RESOURCE(electron_bg_texteditor_png);
INCLUDE_EXTERN_RESOURCE(electron_context_png);
INCLUDE_EXTERN_RESOURCE(electron_context_more_png);
INCLUDE_EXTERN_RESOURCE(electron_cover_png);
INCLUDE_EXTERN_RESOURCE(electron_dialog_png);
INCLUDE_EXTERN_RESOURCE(electron_fastforward_png);
INCLUDE_EXTERN_RESOURCE(electron_fastrewind_png);
INCLUDE_EXTERN_RESOURCE(electron_file_icon_png);
INCLUDE_EXTERN_RESOURCE(electron_folder_icon_png);
INCLUDE_EXTERN_RESOURCE(electron_ftp_png);
INCLUDE_EXTERN_RESOURCE(electron_image_icon_png);
INCLUDE_EXTERN_RESOURCE(electron_pause_png);
INCLUDE_EXTERN_RESOURCE(electron_play_png);
INCLUDE_EXTERN_RESOURCE(electron_sfo_icon_png);
INCLUDE_EXTERN_RESOURCE(electron_text_icon_png);
INCLUDE_EXTERN_RESOURCE(electron_settings_png);

INCLUDE_EXTERN_RESOURCE(umass_skprx);

extern unsigned char _binary_modules_kernel_kernel_skprx_start;
extern unsigned char _binary_modules_kernel_kernel_skprx_size;
extern unsigned char _binary_modules_user_user_suprx_start;
extern unsigned char _binary_modules_user_user_suprx_size;
extern unsigned char _binary_modules_patch_patch_skprx_start;
extern unsigned char _binary_modules_patch_patch_skprx_size;
extern unsigned char _binary_modules_usbdevice_usbdevice_skprx_start;
extern unsigned char _binary_modules_usbdevice_usbdevice_skprx_size;

#define DEFAULT_FILE(path, name, replace) { path, (void *)&_binary_resources_##name##_start, (int)&_binary_resources_##name##_size, replace }

static DefaultFile default_files[] = {
  DEFAULT_FILE("language/english_us.txt", english_us_txt, 0),

  DEFAULT_FILE("theme/theme.txt", theme_txt, 0),

  DEFAULT_FILE("theme/Default/colors.txt", default_colors_txt, 1),
  DEFAULT_FILE("theme/Default/archive_icon.png", default_archive_icon_png, 1),
  DEFAULT_FILE("theme/Default/audio_icon.png", default_audio_icon_png, 1),
  DEFAULT_FILE("theme/Default/battery.png", default_battery_png, 1),
  DEFAULT_FILE("theme/Default/battery_bar_charge.png", default_battery_bar_charge_png, 1),
  DEFAULT_FILE("theme/Default/battery_bar_green.png", default_battery_bar_green_png, 1),
  DEFAULT_FILE("theme/Default/battery_bar_red.png", default_battery_bar_red_png, 1),
  DEFAULT_FILE("theme/Default/cover.png", default_cover_png, 1),
  DEFAULT_FILE("theme/Default/fastforward.png", default_fastforward_png, 1),
  DEFAULT_FILE("theme/Default/fastrewind.png", default_fastrewind_png, 1),
  DEFAULT_FILE("theme/Default/file_icon.png", default_file_icon_png, 1),
  DEFAULT_FILE("theme/Default/file_symlink_icon.png",default_file_symlink_icon_png, 1),
  DEFAULT_FILE("theme/Default/folder_icon.png", default_folder_icon_png, 1),
  DEFAULT_FILE("theme/Default/folder_symlink_icon.png",default_folder_symlink_icon_png,  1),
  DEFAULT_FILE("theme/Default/ftp.png", default_ftp_png, 1),
  DEFAULT_FILE("theme/Default/image_icon.png", default_image_icon_png, 1),
  DEFAULT_FILE("theme/Default/pause.png", default_pause_png, 1),
  DEFAULT_FILE("theme/Default/play.png", default_play_png, 1),
  DEFAULT_FILE("theme/Default/sfo_icon.png", default_sfo_icon_png, 1),
  DEFAULT_FILE("theme/Default/text_icon.png", default_text_icon_png, 1),

  DEFAULT_FILE("theme/Electron/colors.txt", electron_colors_txt, 1),
  DEFAULT_FILE("theme/Electron/archive_icon.png", electron_archive_icon_png, 1),
  DEFAULT_FILE("theme/Electron/audio_icon.png", electron_audio_icon_png, 1),
  DEFAULT_FILE("theme/Electron/battery.png", electron_battery_png, 1),
  DEFAULT_FILE("theme/Electron/battery_bar_charge.png", electron_battery_bar_charge_png, 1),
  DEFAULT_FILE("theme/Electron/battery_bar_green.png", electron_battery_bar_green_png, 1),
  DEFAULT_FILE("theme/Electron/battery_bar_red.png", electron_battery_bar_red_png, 1),
  DEFAULT_FILE("theme/Electron/bg_audioplayer.png", electron_bg_audioplayer_png, 1),
  DEFAULT_FILE("theme/Electron/bg_browser.png", electron_bg_browser_png, 1),
  DEFAULT_FILE("theme/Electron/bg_hexeditor.png", electron_bg_hexeditor_png, 1),
  DEFAULT_FILE("theme/Electron/bg_photoviewer.png", electron_bg_photoviewer_png, 1),
  DEFAULT_FILE("theme/Electron/bg_texteditor.png", electron_bg_texteditor_png, 1),
  DEFAULT_FILE("theme/Electron/context.png", electron_context_png, 1),
  DEFAULT_FILE("theme/Electron/context_more.png", electron_context_more_png, 1),
  DEFAULT_FILE("theme/Electron/cover.png", electron_cover_png, 1),
  DEFAULT_FILE("theme/Electron/dialog.png", electron_dialog_png, 1),
  DEFAULT_FILE("theme/Electron/fastforward.png", electron_fastforward_png, 1),
  DEFAULT_FILE("theme/Electron/fastrewind.png", electron_fastrewind_png, 1),
  DEFAULT_FILE("theme/Electron/file_icon.png", electron_file_icon_png, 1),
  DEFAULT_FILE("theme/Electron/folder_icon.png", electron_folder_icon_png, 1),
  DEFAULT_FILE("theme/Electron/ftp.png", electron_ftp_png, 1),
  DEFAULT_FILE("theme/Electron/image_icon.png", electron_image_icon_png, 1),
  DEFAULT_FILE("theme/Electron/pause.png", electron_pause_png, 1),
  DEFAULT_FILE("theme/Electron/play.png", electron_play_png, 1),
  DEFAULT_FILE("theme/Electron/sfo_icon.png", electron_sfo_icon_png, 1),
  DEFAULT_FILE("theme/Electron/text_icon.png", electron_text_icon_png, 1),
  DEFAULT_FILE("theme/Electron/settings.png", electron_settings_png, 1),

  DEFAULT_FILE("module/umass.skprx", umass_skprx, 1),
  
  { "module/kernel.skprx",    (void *)&_binary_modules_kernel_kernel_skprx_start,
                                               (int)&_binary_modules_kernel_kernel_skprx_size, 1 },
  { "module/user.suprx",      (void *)&_binary_modules_user_user_suprx_start,
                                               (int)&_binary_modules_user_user_suprx_size, 1 },
  { "module/patch.skprx",     (void *)&_binary_modules_patch_patch_skprx_start,
                                               (int)&_binary_modules_patch_patch_skprx_size, 1 },
  { "module/usbdevice.skprx", (void *)&_binary_modules_usbdevice_usbdevice_skprx_start,
                                               (int)&_binary_modules_usbdevice_usbdevice_skprx_size, 1 },
};

char vitashell_titleid[12];

int is_safe_mode = 0;

char vitashell_base[16] = VITASHELL_UX0_BASE;
char vitashell_lastdir[64];
char vitashell_bookmarks_path[64];
char vitashell_recent_path[64];

SceUID patch_modid = -1, kernel_modid = -1, user_modid = -1;

// System params
int language = 0, enter_button = 0, date_format = 0, time_format = 0;

int isSafeMode() {
  return is_safe_mode;
}

static void initSceAppUtil() {
  // Init SceAppUtil
  SceAppUtilInitParam init_param;
  SceAppUtilBootParam boot_param;
  memset(&init_param, 0, sizeof(SceAppUtilInitParam));
  memset(&boot_param, 0, sizeof(SceAppUtilBootParam));
  sceAppUtilInit(&init_param, &boot_param);

  // Mount
  sceAppUtilMusicMount();
  sceAppUtilPhotoMount();

  // System params
  sceAppUtilSystemParamGetInt(SCE_SYSTEM_PARAM_ID_LANG, &language);
  sceAppUtilSystemParamGetInt(SCE_SYSTEM_PARAM_ID_ENTER_BUTTON, &enter_button);
  sceAppUtilSystemParamGetInt(SCE_SYSTEM_PARAM_ID_DATE_FORMAT, &date_format);
  sceAppUtilSystemParamGetInt(SCE_SYSTEM_PARAM_ID_TIME_FORMAT, &time_format);

  // Set common dialog config
  SceCommonDialogConfigParam config;
  sceCommonDialogConfigParamInit(&config);
  sceAppUtilSystemParamGetInt(SCE_SYSTEM_PARAM_ID_LANG, (int *)&config.language);
  sceAppUtilSystemParamGetInt(SCE_SYSTEM_PARAM_ID_ENTER_BUTTON, (int *)&config.enterButtonAssign);
  sceCommonDialogSetConfigParam(&config);
}

static void finishSceAppUtil() {
  // Unmount
  sceAppUtilPhotoUmount();
  sceAppUtilMusicUmount();

  // Shutdown AppUtil
  sceAppUtilShutdown();
}

static int isKoreanChar(unsigned int c) {
  unsigned short ch = c;

  // Hangul compatibility jamo block
  if (0x3130 <= ch && ch <= 0x318F) {
    return 1;
  }

  // Hangul syllables block
  if (0xAC00 <= ch && ch <= 0xD7AF) {
    return 1;
  }

  // Korean won sign
  if (ch == 0xFFE6) {
    return 1;
  }

  return 0;
}

static int isLatinChar(unsigned int c) {
  unsigned short ch = c;

  // Basic latin block + latin-1 supplement block
  if (ch <= 0x00FF) {
    return 1;
  }

  // Cyrillic block
  if (0x0400 <= ch && ch <= 0x04FF) {
    return 1;
  }

  return 0;
}

vita2d_pgf *loadSystemFonts() {
  vita2d_system_pgf_config configs[] = {
    { SCE_FONT_LANGUAGE_KOREAN, isKoreanChar },
    { SCE_FONT_LANGUAGE_LATIN, isLatinChar },
    { SCE_FONT_LANGUAGE_DEFAULT, NULL },
  };

  return vita2d_load_system_pgf(3, configs);
}

static void initVita2dLib() {
  vita2d_init();
}

static void finishVita2dLib() {
  vita2d_free_pgf(font);
  vita2d_fini();

  font = NULL;
}

static int initSQLite() {
  return sqlite_init();
}

static int finishSQLite() {
  return sqlite_exit();
}

#define NET_MEMORY_SIZE (4 * 1024 * 1024)

static char *net_memory = NULL;

static void initNet() {
  net_memory = malloc(NET_MEMORY_SIZE);

  SceNetInitParam param;
  param.memory = net_memory;
  param.size = NET_MEMORY_SIZE;
  param.flags = 0;

  sceNetInit(&param);
  sceNetCtlInit();

  sceSslInit(300 * 1024);
  sceHttpInit(40 * 1024);

  sceHttpsDisableOption(SCE_HTTPS_FLAG_SERVER_VERIFY);

  sceNetAdhocInit();

  SceNetAdhocctlAdhocId adhocId;
  memset(&adhocId, 0, sizeof(SceNetAdhocctlAdhocId));
  adhocId.type = SCE_NET_ADHOCCTL_ADHOCTYPE_RESERVED;
  memcpy(&adhocId.data[0], VITASHELL_TITLEID, SCE_NET_ADHOCCTL_ADHOCID_LEN);
  sceNetAdhocctlInit(&adhocId);
}

static void finishNet() {
  sceNetAdhocctlTerm();
  sceNetAdhocTerm();
  sceSslTerm();
  sceHttpTerm();
  sceNetCtlTerm();
  sceNetTerm();
  free(net_memory);
}

static int isDeviceAvailable(const char *device) {
  SceIoStat stat;
  memset(&stat, 0, sizeof(SceIoStat));
  return sceIoGetstat(device, &stat) >= 0;
}

static void initVitaShellBase() {
  // Use the folder on ux0: by default. If ux0: is not available (e.g. no
  // memory card or a broken one), fall back to the folder on ur0: so that
  // VitaShell can still load its modules and be used to repair the system.
  // In safe mode ux0: is the only accessible device, so always keep it.
  if (!is_safe_mode && !isDeviceAvailable("ux0:") && isDeviceAvailable("ur0:"))
    strcpy(vitashell_base, VITASHELL_UR0_BASE);
  else
    strcpy(vitashell_base, VITASHELL_UX0_BASE);

  snprintf(vitashell_lastdir, sizeof(vitashell_lastdir), "%s/internal/lastdir.txt", vitashell_base);
  snprintf(vitashell_bookmarks_path, sizeof(vitashell_bookmarks_path), "%s/bookmarks/", vitashell_base);
  snprintf(vitashell_recent_path, sizeof(vitashell_recent_path), "%s/recent/", vitashell_base);
}

void getVitaShellPath(char *path, int size, const char *relative_path) {
  if (relative_path[0] == '\0')
    snprintf(path, size, "%s", vitashell_base);
  else
    snprintf(path, size, "%s/%s", vitashell_base, relative_path);
}

void installDefaultFiles() {
  static const char *folders[] = {
    "",
    "internal",
    "language",
    "module",
    "theme",
    "theme/Default",
    "theme/Electron",
  };

  char path[MAX_PATH_LENGTH];
  int i;

  // Make VitaShell folders
  for (i = 0; i < (sizeof(folders) / sizeof(char *)); i++) {
    getVitaShellPath(path, sizeof(path), folders[i]);
    sceIoMkdir(path, 0777);
  }

  // Write default files if they don't exist
  for (i = 0; i < (sizeof(default_files) / sizeof(DefaultFile)); i++) {
    getVitaShellPath(path, sizeof(path), default_files[i].path);

    SceIoStat stat;
    memset(&stat, 0, sizeof(stat));
    if (sceIoGetstat(path, &stat) < 0 || (default_files[i].replace && (int)stat.st_size != default_files[i].size))
      WriteFile(path, default_files[i].buffer, default_files[i].size);
  }
}

void initVitaShell() {
  // Set CPU to 444mhz
  scePowerSetArmClockFrequency(444);

  // Init SceShellUtil events
  sceShellUtilInitEvents(0);

  // Prevent automatic CMA connection
  sceShellUtilLock(SCE_SHELL_UTIL_LOCK_TYPE_USB_CONNECTION);

  // Get titleid
  memset(vitashell_titleid, 0, sizeof(vitashell_titleid));
  sceAppMgrAppParamGetString(sceKernelGetProcessId(), 12, vitashell_titleid, sizeof(vitashell_titleid));

  // Allow writing to ux0:app/VITASHELL
  sceAppMgrUmount("app0:");
  sceAppMgrUmount("savedata0:");

  // Is safe mode
  if (sceIoDevctl("ux0:", 0x3001, NULL, 0, NULL, 0) == 0x80010030)
    is_safe_mode = 1;

  // Select the VitaShell data folder (ux0: or ur0:)
  initVitaShellBase();

  // Set sampling mode
  sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);

  // Load modules
  sceSysmoduleLoadModule(SCE_SYSMODULE_VIDEO_EXPORT);
  sceSysmoduleLoadModule(SCE_SYSMODULE_PGF);
  sceSysmoduleLoadModule(SCE_SYSMODULE_MUSIC_EXPORT);
  sceSysmoduleLoadModule(SCE_SYSMODULE_PHOTO_EXPORT);
  sceSysmoduleLoadModule(SCE_SYSMODULE_NET);
  sceSysmoduleLoadModule(SCE_SYSMODULE_HTTPS);
  sceSysmoduleLoadModule(SCE_SYSMODULE_PSPNET_ADHOC);
  sceSysmoduleLoadModule(SCE_SYSMODULE_SQLITE);

  // Init
  vitaAudioInit(0x40);
  initVita2dLib();
  initSceAppUtil();
  initNet();
  initQR();
  initSQLite();

  // Init power tick thread
  initPowerTickThread();

  // Delete VitaShell updater if available
  if (checkAppExist("VSUPDATER")) {
    deleteApp("VSUPDATER");
  }

  // Install default files
  installDefaultFiles();

  // Load modules
  char module_path[MAX_PATH_LENGTH];
  int search_unk[2];
  SceUID search_modid;
  search_modid = _vshKernelSearchModuleByName("VitaShellPatch", search_unk);
  if(search_modid < 0) {
    getVitaShellPath(module_path, sizeof(module_path), "module/patch.skprx");
    patch_modid = taiLoadKernelModule(module_path, 0, NULL);
    if (patch_modid >= 0) {
      int res = taiStartKernelModule(patch_modid, 0, NULL, 0, NULL, NULL);
      if (res < 0)
        taiStopUnloadKernelModule(patch_modid, 0, NULL, 0, NULL, NULL);
    }
  }
  search_modid = _vshKernelSearchModuleByName("VitaShellKernel2", search_unk);
  if(search_modid < 0) {
    getVitaShellPath(module_path, sizeof(module_path), "module/kernel.skprx");
    kernel_modid = taiLoadKernelModule(module_path, 0, NULL);
    if (kernel_modid >= 0) {
      int res = taiStartKernelModule(kernel_modid, 0, NULL, 0, NULL, NULL);
      if (res < 0)
        taiStopUnloadKernelModule(kernel_modid, 0, NULL, 0, NULL, NULL);
    }
  }
  getVitaShellPath(module_path, sizeof(module_path), "module/user.suprx");
  user_modid = sceKernelLoadStartModule(module_path, 0, NULL, 0, NULL, NULL);

  // clear up recent folder frequently
  SceIoStat stat;
  SceDateTime now;
  sceRtcGetCurrentClock(&now, 0);
  int res = sceIoGetstat(VITASHELL_RECENT_PATH, &stat);
  if (res >= 0) {
    if (now.year * 365 + now.day - stat.st_ctime.year * 365 - stat.st_ctime.day
        >= VITASHELL_RECENT_PATH_DELETE_INTERVAL_DAYS) {
      removePath(VITASHELL_RECENT_PATH, 0);
    }
  }

  if (!checkFolderExist(VITASHELL_BOOKMARKS_PATH)) {
    sceIoMkdir(VITASHELL_BOOKMARKS_PATH, 0777);
  }
  if (!checkFolderExist(VITASHELL_RECENT_PATH)) {
    sceIoMkdir(VITASHELL_RECENT_PATH, 0777);
  }
  time_last_recent_files = 0;
  time_last_bookmarks = 0;
}

void finishVitaShell() {
  // Finish
  finishSQLite();
  finishNet();
  finishSceAppUtil();
  finishVita2dLib();
  finishQR();
  vitaAudioShutdown();
  
  // Unload modules
  sceSysmoduleUnloadModule(SCE_SYSMODULE_SQLITE);
  sceSysmoduleUnloadModule(SCE_SYSMODULE_PSPNET_ADHOC);
  sceSysmoduleUnloadModule(SCE_SYSMODULE_HTTPS);
  sceSysmoduleUnloadModule(SCE_SYSMODULE_NET);
  sceSysmoduleUnloadModule(SCE_SYSMODULE_PHOTO_EXPORT);
  sceSysmoduleUnloadModule(SCE_SYSMODULE_MUSIC_EXPORT);
  sceSysmoduleUnloadModule(SCE_SYSMODULE_PGF);
  sceSysmoduleUnloadModule(SCE_SYSMODULE_VIDEO_EXPORT);
}
