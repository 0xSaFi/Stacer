#ifndef GNOMESCHEMA_H
#define GNOMESCHEMA_H

namespace GSchemaPaths {
    const QString Unity = "/org/compiz/profiles/unity/plugins/unityshell/";
    const QString UnityLauncher = "/com/canonical/unity/launcher/";
    const QString OpenGL = "/org/compiz/profiles/unity/plugins/opengl/";
    const QString Core = "/org/compiz/profiles/unity/plugins/core/";
}

namespace GSchemas {
    namespace Unity {
        const QString Shell = "org.compiz.unityshell";
        const QString Launcher = "com.canonical.Unity.Launcher";
        const QString Lens = "com.canonical.Unity.Lenses";
        const QString AppLens = "com.canonical.Unity.ApplicationsLens";
        const QString FileLens = "com.canonical.Unity.FilesLens";
        const QString DateTime = "com.canonical.indicator.datetime";
        const QString Sound = "com.canonical.indicator.sound";
        const QString Session = "com.canonical.indicator.session";
    }
    namespace Window {
        const QString OpenGL = "org.compiz.opengl";
        const QString Core = "org.compiz.core";
        const QString Preferences = "org.gnome.desktop.wm.preferences";
    }
}

namespace GSchemaKeys {
    namespace Unity {
        const QString LauncherHideMode = "launcher-hide-mode";
        const QString RevealTrigger = "reveal-trigger";
        const QString EdgeResponsiveness = "edge-responsiveness";
        const QString LauncherMinimizeApp = "launcher-minimize-window";
        const QString LauncherOpacity = "launcher-opacity";
        const QString LauncherVisibility = "num-launchers";
        const QString LauncherPosition = "launcher-position";
        const QString LauncherIconSize = "icon-size";
        const QString DashBlur = "dash-blur-experimental";
        const QString SearchOnlineResource = "remote-content-search";
        const QString DisplayAvailableApps = "display-available-apps";
        const QString DisplayRecentApps = "display-recent-apps";
        const QString EnableSearchFile = "use-locate";
        const QString PanelOpacity = "panel-opacity";
        const QString ShowDateTime = "show-clock";
        const QString TimeFormat = "time-format";
        const QString ShowSeconds = "show-seconds";
        const QString ShowDate = "show-date";
        const QString ShowDay = "show-day";
        const QString ShowCalendar = "show-calendar";
        const QString ShowVolume = "visible";
        const QString ShowMyName = "show-real-name-on-panel";
    }

    namespace Window {
        const QString TextureQuality = "texture-filter";
        const QString HorizontalWorkSize = "hsize";
        const QString VerticalWorkSize = "vsize";
        const QString RaiseOnClick = "raise-on-click";
        const QString FocusMode = "focus-mode";
        const QString ActionDoubleClick = "action-double-click-titlebar";
        const QString ActionMiddleClick = "action-middle-click-titlebar";
        const QString ActionRightClick = "action-right-click-titlebar";
    }
}

namespace GValues {
    enum RevealLocation {
        Left,
        TopLeft
    };

    enum LauncherVisibility {
        AllDesktop,
        PrimaryDesktop
    };
}

#endif // GNOMESCHEMA_H
