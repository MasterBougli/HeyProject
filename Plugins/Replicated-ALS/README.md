# Replicated - Advanced Locomotion System
![Image](https://github.com/eyupalemdar/Replicated-ALS/blob/main/Resources/Readme_Content_2.gif)

Replicated version of [Advanced Locomotion System V4](https://www.unrealengine.com/marketplace/en-US/product/advanced-locomotion-system-v1) for **Unreal Engine 4.26**. Almost fully implemented in Blueprint (%5 C++ and %95 Blueprint scripting) to preserve original ALS framework and power of Blueprint scripting. Inspired by the [community](https://github.com/dyanikoglu/ALS-Community) project.

## Showcase Project
You can checkout the [example project](https://github.com/eyupalemdar/TestReplicatedALS) and examine how the ReplicatedALS plugin is used.

## Supported Platforms
- Windows

*macOS, iOS, Linux, Android, and console builds are not tested. Use the plugin on those platforms with your own risk*.

## Features
- Based on latest marketplace release.
- %5 impelemented in C++ and %95 impelemented in Blueprint scripting.
- Full replication support with low bandwidth usage.
- Plugin structure.
- Highly optimized for production.
- Lots of bug fixes additional to marketplace version.

## Known Issues & Discussion
- See [Issues](https://github.com/eyupalemdar/Replicated-ALS/issues) section for list of known issues.
- See [Discussions](https://github.com/eyupalemdar/Replicated-ALS/discussions) section to discuss anything about the plugin, and ask questions. Please do not open an issue to ask questions about the plugin.

## Setting Up The Plugin
- **C++ project is a requirement. BP projects are currently not supported**.
- Clone the repository inside your project's `Plugins` folder, or download the latest release and extract it into your project's `Plugins` folder.
- Put `Config/DefaultInput.ini` from the plugin folder inside your project's config folder. If your project already have this `.ini` file, merge it into yours.
- Put `Config/DefaultEngine.ini` from the plugin folder inside your project's config folder or merge `[/Script/Engine.CollisionProfile]` section in ini file with your project's `DefaultEngine.ini` file.
- Regenerate Visual Studio project files and build your project.
- Launch the project, and enable plugin content viewer as seen below. This will show contents of the plugin in your content browser:
![image](https://github.com/eyupalemdar/Replicated-ALS/blob/main/Resources/Readme_Content_1.png)

## License & Contribution
**Source code** of the plugin is licensed under MIT license, and other developers are encouraged to fork the repository, open issues & pull requests to help the development.
