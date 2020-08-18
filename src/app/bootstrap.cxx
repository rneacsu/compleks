#include <Windows.h>
#include <iostream>

extern int main(int argc, char *argv[]);

int WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nShowCmd
) {
    int argc;
    WCHAR **argv;

    hInstance, hPrevInstance, lpCmdLine, nShowCmd;

    argv = CommandLineToArgvW(GetCommandLineW(), &argc);

    if (!argv) {
        std::cerr << "Could not parse program arguments\n";

        return GetLastError();
    }

    for (int i = 0; i < argc; i++) {
        int j = 0;
        do {
            ((char *)argv[i])[j] = (char) argv[i][j];
        } while (argv[i][j++]);

    }

    return main(argc, (char **)argv);
}