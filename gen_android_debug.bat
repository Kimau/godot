scons platform=android target=template_debug arch=arm32
scons platform=android target=template_debug arch=arm64
cd platform/android/java
.\gradlew generateGodotTemplates