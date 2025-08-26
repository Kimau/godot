#include "fast_text_gizmo.h"

#include "editor/plugins/node_3d_editor_plugin.h"
#include "../fast_text.h"

FastTextGizmoPlugin::FastTextGizmoPlugin() {
}


bool FastTextGizmoPlugin::has_gizmo(Node3D *p_spatial) {
	return Object::cast_to<FastText>(p_spatial) != nullptr;
}


String FastTextGizmoPlugin::get_gizmo_name() const {
	return "FastText";
}


int FastTextGizmoPlugin::get_priority() const {
	return -1;
}


bool FastTextGizmoPlugin::can_be_hidden() const {
	return false;
}


void FastTextGizmoPlugin::redraw(EditorNode3DGizmo *p_gizmo) {
	FastText *text = Object::cast_to<FastText>(p_gizmo->get_node_3d());


	p_gizmo->clear();
    
    Ref<TriangleMesh> tm = text->get_triangle_mesh();
	if (tm.is_valid()) {
		p_gizmo->add_collision_triangles(tm);
	}

}

////


FastTextPlugin::FastTextPlugin() {
	Ref<FastTextGizmoPlugin> gizmo_plugin = Ref<FastTextGizmoPlugin>(memnew(FastTextGizmoPlugin));
	Node3DEditor::get_singleton()->add_gizmo_plugin(gizmo_plugin);
}