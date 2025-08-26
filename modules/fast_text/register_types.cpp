/* register_types.cpp */

#include "register_types.h"
#include "godot_cpp/core/class_db.hpp"

#include "fast_text_shared.h"
#include "fast_text.h"

#ifdef TOOLS_ENABLED
#include "editor/fast_text_gizmo.h"
#endif

using namespace godot;

void initialize_fast_text_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
		ClassDB::register_class<FastTextSharedResource>();
		ClassDB::register_class<FastText>();
	}

#ifdef TOOLS_ENABLED
	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR) {
		EditorPlugins::add_by_type<FastTextPlugin>();
	}
#endif
}

void uninitialize_fast_text_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

