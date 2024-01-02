//Both the Desktop+ dashboard overlay and the UI hold a copy of the current configuration
//Both load the initial state from config.ini
//Changes are generally done by the UI and sent to the dashboard application via Win32 messages
//Only the UI writes to config.ini

#pragma once

#include <string>
#include <vector>
#define NOMINMAX
#include <windows.h>

#include "Matrices.h"
#include "Actions.h"
#include "Ini.h"

//Settings enums
//These IDs are also passed via IPC
//configid_*_state entries are not stored/persistent and are just a simpler way to sync state

enum ConfigID_Bool
{
    configid_bool_overlay_detached,
    configid_bool_overlay_name_custom,
    configid_bool_overlay_enabled,
    configid_bool_overlay_width_unscaled,
    configid_bool_overlay_3D_swapped,
    configid_bool_overlay_gazefade_enabled,
    configid_bool_overlay_input_enabled,
    configid_bool_overlay_update_invisible,
    configid_bool_overlay_floatingui_enabled,
    configid_bool_overlay_floatingui_desktops_enabled,
    configid_bool_overlay_actionbar_enabled,
    configid_bool_overlay_actionbar_order_use_global,
    configid_bool_overlay_MAX,
    configid_bool_interface_no_ui,
    configid_bool_interface_no_notification_icon,
    configid_bool_interface_large_style,
    configid_bool_interface_dim_ui,
    configid_bool_interface_mainbar_desktop_include_all,
    configid_bool_interface_warning_compositor_res_hidden,
    configid_bool_interface_warning_compositor_quality_hidden,
    configid_bool_interface_warning_process_elevation_hidden,
    configid_bool_interface_warning_elevated_mode_hidden,
    configid_bool_interface_warning_welcome_hidden,
    configid_bool_performance_rapid_laser_pointer_updates,
    configid_bool_performance_single_desktop_mirroring,
    configid_bool_performance_monitor_large_style,
    configid_bool_performance_monitor_show_graphs,
    configid_bool_performance_monitor_show_time,
    configid_bool_performance_monitor_show_cpu,
    configid_bool_performance_monitor_show_gpu,
    configid_bool_performance_monitor_show_fps,
    configid_bool_performance_monitor_show_battery,
    configid_bool_performance_monitor_show_trackers,
    configid_bool_performance_monitor_show_vive_wireless,
    configid_bool_performance_monitor_disable_gpu_counters,
    configid_bool_input_global_hmd_pointer,
    configid_bool_input_mouse_render_cursor,
    configid_bool_input_mouse_render_intersection_blob,
    configid_bool_input_mouse_hmd_pointer_override,
    configid_bool_input_keyboard_helper_enabled,
    configid_bool_windows_auto_focus_scene_app_dashboard,
    configid_bool_windows_winrt_auto_focus,
    configid_bool_windows_winrt_keep_on_screen,
    configid_bool_windows_winrt_auto_size_overlay,
    configid_bool_windows_winrt_auto_focus_scene_app,
    configid_bool_misc_no_steam,                              //Restarts without Steam when it detects to have been launched by Steam
    configid_bool_misc_uiaccess_was_enabled,                  //Tracks if UIAccess was enabled to show a warning after it isn't anymore due to updates or modified executable
    configid_bool_misc_apply_steamvr2_dashboard_offset,       //Applies backward compatibility offset transform when the SteamVR 2 dashboard is detected
    configid_bool_state_overlay_dragmode,
    configid_bool_state_overlay_selectmode,
    configid_bool_state_overlay_dragselectmode_show_hidden,   //True if mode is from a popup
    configid_bool_state_window_focused_process_elevated,
    configid_bool_state_performance_stats_active,             //Only count when the stats are visible
    configid_bool_state_performance_gpu_copy_active,
    configid_bool_state_misc_process_elevated,                //True if the dashboard application is running with admin privileges
    configid_bool_state_misc_elevated_mode_active,            //True if the elevated mode process is running
    configid_bool_state_misc_process_started_by_steam,
    configid_bool_state_misc_uiaccess_enabled,
    configid_bool_MAX
};

enum ConfigID_Int
{
    configid_int_overlay_desktop_id,                        //-1 is combined desktop, -2 is a default value that initializes crop to desktop 0
    configid_int_overlay_capture_source,
    configid_int_overlay_winrt_desktop_id,                  //-1 is combined desktop, -2 is unset
    configid_int_overlay_crop_x,
    configid_int_overlay_crop_y,
    configid_int_overlay_crop_width,
    configid_int_overlay_crop_height,
    configid_int_overlay_3D_mode,
    configid_int_overlay_detached_display_mode,
    configid_int_overlay_detached_origin,
    configid_int_overlay_update_limit_override_mode,
    configid_int_overlay_update_limit_override_fps,
    configid_int_overlay_group_id,
    configid_int_overlay_state_content_width,
    configid_int_overlay_state_content_height,
    configid_int_overlay_MAX,
    configid_int_interface_overlay_current_id,
    configid_int_interface_mainbar_desktop_listing,
    configid_int_interface_background_color,
    configid_int_interface_background_color_display_mode,
    configid_int_interface_wmr_ignore_vscreens,             //This assumes the WMR virtual screens are the 3 last ones... (-1 means auto/unset which is the value non-WMR users get)
    configid_int_input_go_home_action_id,
    configid_int_input_go_back_action_id,
    configid_int_input_shortcut01_action_id,
    configid_int_input_shortcut02_action_id,
    configid_int_input_shortcut03_action_id,
    configid_int_input_hotkey01_modifiers,
    configid_int_input_hotkey01_keycode,
    configid_int_input_hotkey01_action_id,
    configid_int_input_hotkey02_modifiers,
    configid_int_input_hotkey02_keycode,
    configid_int_input_hotkey02_action_id,
    configid_int_input_hotkey03_modifiers,
    configid_int_input_hotkey03_keycode,
    configid_int_input_hotkey03_action_id,
    configid_int_input_mouse_dbl_click_assist_duration_ms,
    configid_int_windows_winrt_dragging_mode,
    configid_int_performance_update_limit_mode,
    configid_int_performance_update_limit_fps,              //This is the enum ID, not the actual number. See ApplySettingUpdateLimiter() code for more info
    configid_int_state_overlay_current_id_override,         //This is used to send config changes to overlays which aren't the current, mainly to avoid the UI switching around (-1 is disabled)
    configid_int_state_action_current,                      //Action changes are synced through a series of individually sent state settings. This one sets the target custom action (ID start 0)
    configid_int_state_action_current_sub,                  //Target variable. 0 = Name, 1 = Function Type. Remaining values depend on the function. Not the cleanest way but easier
    configid_int_state_action_value_int,                    //to set up with existing IPC stuff
    configid_int_state_mouse_dbl_click_assist_duration_ms,  //Internally used value, which will replace -1 with the current double-click delay automatically
    configid_int_state_keyboard_visible_for_overlay_id,     //-1 = None
    configid_int_state_keyboard_modifiers,                  //Keyboard modifier state when keyboard helper is enabled and visible (allows UI seeing state while elevated app is in focus)
    configid_int_state_performance_duplication_fps,
    configid_int_state_interface_desktop_count,             //Count of desktops after optionally filtering virtual WMR displays
    configid_int_state_interface_floating_ui_hovered_id,    //Floating UI target overlay ID set only while the laser pointer is pointing at the Floating UI overlay. -1 = None
    configid_int_MAX
};

enum ConfigID_Float
{
    configid_float_overlay_width,
    configid_float_overlay_curvature,
    configid_float_overlay_opacity,
    configid_float_overlay_brightness,
    configid_float_overlay_offset_right,
    configid_float_overlay_offset_up,
    configid_float_overlay_offset_forward,
    configid_float_overlay_gazefade_distance,
    configid_float_overlay_gazefade_rate,
    configid_float_overlay_gazefade_opacity,
    configid_float_overlay_update_limit_override_ms,
    configid_float_overlay_MAX,
    configid_float_input_detached_interaction_max_distance,
    configid_float_input_global_hmd_pointer_max_distance,
    configid_float_interface_last_vr_ui_scale,
    configid_float_performance_update_limit_ms,
    configid_float_MAX
};

enum ConfigID_IntPtr
{
    configid_intptr_overlay_state_winrt_hwnd,               //HWNDs are technically always in 32-bit range, but avoiding truncation warnings and perhaps some other issues here
    configid_intptr_overlay_MAX,
    configid_intptr_MAX = configid_intptr_overlay_MAX
};

enum ConfigID_String
{
    configid_str_overlay_winrt_last_window_title,
    configid_str_overlay_winrt_last_window_exe_name,
    configid_str_overlay_MAX,
    configid_str_state_detached_transform_current,
    configid_str_state_action_value_string,
    configid_str_state_ui_keyboard_string,           //SteamVR keyboard input for the UI application
    configid_str_state_dashboard_error_string,       //Error messages are displayed in VR through the UI app
    configid_str_state_profile_name_load,            //Name of the profile to load 
	configid_str_MAX
};

//Actually stored as ints, but still have this for readability
enum OverlayCaptureSource
{
    ovrl_capsource_desktop_duplication,
    ovrl_capsource_winrt_capture,
    ovrl_capsource_ui
};

enum Overlay3DMode
{
    ovrl_3Dmode_none,
    ovrl_3Dmode_hsbs,
    ovrl_3Dmode_sbs,
    ovrl_3Dmode_hou,
    ovrl_3Dmode_ou
};

enum OverlayDisplayMode
{
    ovrl_dispmode_always,
    ovrl_dispmode_dashboard,
    ovrl_dispmode_scene,
    ovrl_dispmode_dplustab
};

enum OverlayOrigin
{
    ovrl_origin_room,
    ovrl_origin_hmd_floor,
    ovrl_origin_seated_universe,
    ovrl_origin_dashboard,
    ovrl_origin_hmd,
    ovrl_origin_right_hand,
    ovrl_origin_left_hand,
    ovrl_origin_aux,        //Tracker or whatever. No proper autodetection of additional devices yet, maybe in the future
    ovrl_origin_MAX         //Used for storing one matrix per origin
};

enum MainbarDesktopListing
{
    mainbar_desktop_listing_none,
    mainbar_desktop_listing_individual,
    mainbar_desktop_listing_cycle
};

enum InterfaceBGColorDisplayMode
{
    ui_bgcolor_dispmode_never,
    ui_bgcolor_dispmode_dplustab,
    ui_bgcolor_dispmode_always
};

enum UpdateLimitMode
{
    update_limit_mode_off,
    update_limit_mode_ms,
    update_limit_mode_fps
};

enum UpdateLimitFPS
{
    update_limit_fps_1,
    update_limit_fps_2,
    update_limit_fps_5,
    update_limit_fps_10,
    update_limit_fps_15,
    update_limit_fps_20,
    update_limit_fps_25,
    update_limit_fps_30,
    update_limit_fps_40,
    update_limit_fps_50
};

enum WindowDraggingMode
{
    window_dragging_none,
    window_dragging_block,
    window_dragging_overlay
};

class OverlayConfigData
{
    public:
        std::string ConfigNameStr;
        bool ConfigBool[configid_bool_overlay_MAX];
        int ConfigInt[configid_int_overlay_MAX];
        float ConfigFloat[configid_float_overlay_MAX];
        intptr_t ConfigIntPtr[configid_intptr_overlay_MAX];
        std::string ConfigStr[configid_str_overlay_MAX];
        Matrix4 ConfigDetachedTransform[ovrl_origin_MAX];
        std::vector<ActionMainBarOrderData> ConfigActionBarOrder;

        OverlayConfigData();
};

class ConfigManager
{
	private:
		bool m_ConfigBool[configid_bool_MAX];
		int m_ConfigInt[configid_int_MAX];
		float m_ConfigFloat[configid_float_MAX];
        //No m_ConfigIntPtr as so far it only contains overlay-specific data
		std::string m_ConfigString[configid_str_MAX];

        ActionManager m_ActionManager;

        std::string m_ApplicationPath;
        std::string m_ExecutableName;
        bool m_IsSteamInstall;

        void LoadOverlayProfile(const Ini& config, unsigned int overlay_id = UINT_MAX);
        void SaveOverlayProfile(Ini& config, unsigned int overlay_id = UINT_MAX);
        void LoadMultiOverlayProfile(const Ini& config, bool clear_existing_overlays = true);
        void SaveMultiOverlayProfile(Ini& config);

        static bool IsUIAccessEnabled();
        static void RemoveScaleFromTransform(Matrix4& transform, float* width);

	public:
		ConfigManager();
		static ConfigManager& Get();

        bool LoadConfigFromFile();
        void SaveConfigToFile();
        void RestoreConfigFromDefault();

        void LoadOverlayProfileDefault(bool multi_overlay = false);
        bool LoadOverlayProfileFromFile(const std::string filename);
        void SaveOverlayProfileToFile(const std::string filename);
        bool LoadMultiOverlayProfileFromFile(const std::string filename, bool clear_existing_overlays = true);
        void SaveMultiOverlayProfileToFile(const std::string filename);
        bool DeleteOverlayProfile(const std::string filename, bool multi_overlay = false);
        std::vector<std::string> GetOverlayProfileList(bool multi_overlay = false);

        static WPARAM GetWParamForConfigID(ConfigID_Bool id);
        static WPARAM GetWParamForConfigID(ConfigID_Int id);
        static WPARAM GetWParamForConfigID(ConfigID_Float id);
        static WPARAM GetWParamForConfigID(ConfigID_IntPtr id);

        void SetConfigBool(ConfigID_Bool id, bool value);
        void SetConfigInt(ConfigID_Int id, int value);
        void SetConfigFloat(ConfigID_Float id, float value);
        void SetConfigIntPtr(ConfigID_IntPtr id, intptr_t value);
        void SetConfigString(ConfigID_String id, const std::string& value);
        bool GetConfigBool(ConfigID_Bool id) const;
        int GetConfigInt(ConfigID_Int id) const;
        float GetConfigFloat(ConfigID_Float id) const;
        intptr_t GetConfigIntPtr(ConfigID_IntPtr id) const;
        const std::string& GetConfigString(ConfigID_String id) const;
        //These are meant for direct use with ImGui widgets
        bool& GetConfigBoolRef(ConfigID_Bool id);
        int& GetConfigIntRef(ConfigID_Int id);
        float& GetConfigFloatRef(ConfigID_Float id);
        intptr_t& GetConfigIntPtrRef(ConfigID_IntPtr id);
        void ResetConfigStateValues();  //Reset all configid_*_state_* settings. Used when restarting a Desktop+ process

        ActionManager& GetActionManager();
        std::vector<CustomAction>& GetCustomActions();
        std::vector<ActionMainBarOrderData>& GetActionMainBarOrder();
        Matrix4& GetOverlayDetachedTransform();

		const std::string& GetApplicationPath() const;
		const std::string& GetExecutableName() const;
        bool IsSteamInstall() const;
};