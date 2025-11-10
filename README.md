# QtSimpleImageViewer

![QtSimpleImageViewer screenshot](./Screenshot_01.png?raw=true)

QtSimpleImageViewer is a lightweight Qt desktop application for culling and inspecting very large folders of images as fast as possible.  
It keeps a high-resolution viewer on the left and an asynchronous thumbnail browser on the right so you can skim thousands of files without blocking the UI.

- Dual-pane workflow for instant context
- Responsive background thumbnail queue with bounded caching
- Pixel-accurate viewer controls and optional non-destructive edits
- Clipboard, export, and batch copy utilities
- Keyboard-friendly navigation for power users

The goal of this project is to provide a snappy, dependency-light inspection tool that feels native on every desktop platform supported by Qt.

## Usage

- Launch the app and choose **File → Open Image…** to point at any file inside the folder you want to explore.
- Scroll through the right-hand thumbnail list to jump across the directory while the main viewer stays in sync.
- Use the toolbar buttons or shortcuts (mouse wheel zoom, click-drag pan, crop overlay) to inspect details without leaving the current pane.

## Features

- **Dual-pane workflow** — full-size preview with zoom/crop tools alongside a scrollable thumbnail list.
- **Asynchronous thumbnails** — previews are decoded in a background queue with a ~256 MB cache so huge folders stay responsive.
- **Precise viewer** — mouse wheel zoom, click-and-drag panning, fit-to-width, 1:1 scaling, pixel readouts, and optional cached scaling.
- **Quick edits** — rotate, flip, manual crop overlay, automatic border crop, and non-destructive save/save-as flows.
- **Clipboard & export utilities** — copy the current image or file path to the clipboard, pick a destination folder, and batch copy selected images with progress tracking.
- **Keyboard-friendly navigation** — `Next` / `Back` actions advance through the folder while thumbnails stay in sync.

## License

This project is released under the [MIT License](https://opensource.org/licenses/MIT).  
See the [LICENSE](LICENSE) file for the full text.

## Requirements

- **Qt 6.5+** (tested with 6.9)  
- CMake 3.16+  
- A C++17 (or later) compiler (GCC, Clang, MSVC all supported)

## Building

```bash
git clone https://github.com/otre99/QtSimpleImageViewer.git
cd QtSimpleImageViewer
```

Open `src/CMakeLists.txt` with Qt Creator, select a kit that points to your Qt 5/6 installation, and build; the IDE generates the `ImageViewer` executable in your configured build directory.

Command-line build:

```bash
mkdir -p build && cd build
cmake -DCMAKE_PREFIX_PATH=/path/to/Qt/lib/cmake ..
cmake --build . --parallel
```

Replace `/path/to/Qt/lib/cmake` with the directory that contains `Qt6Config.cmake` or `Qt5Config.cmake`. The resulting `ImageViewer` binary lives inside `build/`.

## Usage

```bash
./ImageViewer
```

After launching from your build directory, pick any image in the folder you want to browse. The app indexes neighboring files immediately so thumbnails keep pace with the main viewer.

## Contributing

Contributions are welcome! Please open an issue or submit a pull request with reproduction steps, screenshots, or feature ideas to keep the conversation productive.

## Acknowledgements

- Built with Qt Widgets and the standard Qt imaging stack
- Async thumbnail cache powered by `QThreadPool`/`QObject` worker patterns
- Additional demo shots: `Screenshot_02.png`, `Screenshot_03.png`
