scons platform=android target=template_release arch=arm32
scons platform=android target=template_release arch=arm64
cd platform/android/java
.\gradlew generateGodotTemplates
cd \dev\godot