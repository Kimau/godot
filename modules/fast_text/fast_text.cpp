/**************************************************************************/
/*  fast_text.cpp                                                          */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "fast_text.h"

#include "scene/main/window.h"
#include "scene/resources/theme.h"
#include "scene/theme/theme_db.h"

void FastText::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_horizontal_alignment", "alignment"), &FastText::set_horizontal_alignment);
	ClassDB::bind_method(D_METHOD("get_horizontal_alignment"), &FastText::get_horizontal_alignment);

	ClassDB::bind_method(D_METHOD("set_vertical_alignment", "alignment"), &FastText::set_vertical_alignment);
	ClassDB::bind_method(D_METHOD("get_vertical_alignment"), &FastText::get_vertical_alignment);

	ClassDB::bind_method(D_METHOD("set_render_priority", "priority"), &FastText::set_render_priority);
	ClassDB::bind_method(D_METHOD("get_render_priority"), &FastText::get_render_priority);

	ClassDB::bind_method(D_METHOD("set_text", "text"), &FastText::set_text);
	ClassDB::bind_method(D_METHOD("get_text"), &FastText::get_text);

	ClassDB::bind_method(D_METHOD("set_text_direction", "direction"), &FastText::set_text_direction);
	ClassDB::bind_method(D_METHOD("get_text_direction"), &FastText::get_text_direction);

	ClassDB::bind_method(D_METHOD("set_language", "language"), &FastText::set_language);
	ClassDB::bind_method(D_METHOD("get_language"), &FastText::get_language);

	ClassDB::bind_method(D_METHOD("set_uppercase", "enable"), &FastText::set_uppercase);
	ClassDB::bind_method(D_METHOD("is_uppercase"), &FastText::is_uppercase);

	ClassDB::bind_method(D_METHOD("set_line_spacing", "line_spacing"), &FastText::set_line_spacing);
	ClassDB::bind_method(D_METHOD("get_line_spacing"), &FastText::get_line_spacing);

	ClassDB::bind_method(D_METHOD("set_autowrap_mode", "autowrap_mode"), &FastText::set_autowrap_mode);
	ClassDB::bind_method(D_METHOD("get_autowrap_mode"), &FastText::get_autowrap_mode);

	ClassDB::bind_method(D_METHOD("set_justification_flags", "justification_flags"), &FastText::set_justification_flags);
	ClassDB::bind_method(D_METHOD("get_justification_flags"), &FastText::get_justification_flags);

	ClassDB::bind_method(D_METHOD("set_width", "width"), &FastText::set_width);
	ClassDB::bind_method(D_METHOD("get_width"), &FastText::get_width);

	ClassDB::bind_method(D_METHOD("set_offset", "offset"), &FastText::set_offset);
	ClassDB::bind_method(D_METHOD("get_offset"), &FastText::get_offset);

	ClassDB::bind_method(D_METHOD("set_modulate", "color"), &FastText::set_modulate);
	ClassDB::bind_method(D_METHOD("get_modulate"), &FastText::get_modulate);

	ClassDB::bind_method(D_METHOD("set_font_shared", "font_shared"), &FastText::set_font_shared);
	ClassDB::bind_method(D_METHOD("get_font_shared"), &FastText::get_font_shared);



	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "offset", PROPERTY_HINT_NONE, "suffix:px"), "set_offset", "get_offset");

	ADD_GROUP("Shared", "");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "font_shared", PROPERTY_HINT_RESOURCE_TYPE, "FastTextSharedResource"), "set_font_shared", "get_font_shared");

	ADD_GROUP("Text", "");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "text", PROPERTY_HINT_MULTILINE_TEXT, ""), "set_text", "get_text");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "horizontal_alignment", PROPERTY_HINT_ENUM, "Left,Center,Right,Fill"), "set_horizontal_alignment", "get_horizontal_alignment");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "vertical_alignment", PROPERTY_HINT_ENUM, "Top,Center,Bottom"), "set_vertical_alignment", "get_vertical_alignment");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "uppercase"), "set_uppercase", "is_uppercase");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "line_spacing", PROPERTY_HINT_NONE, "suffix:px"), "set_line_spacing", "get_line_spacing");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "autowrap_mode", PROPERTY_HINT_ENUM, "Off,Arbitrary,Word,Word (Smart)"), "set_autowrap_mode", "get_autowrap_mode");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "justification_flags", PROPERTY_HINT_FLAGS, "Kashida Justification:1,Word Justification:2,Justify Only After Last Tab:8,Skip Last Line:32,Skip Last Line With Visible Characters:64,Do Not Skip Single Line:128"), "set_justification_flags", "get_justification_flags");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "width", PROPERTY_HINT_NONE, "suffix:px"), "set_width", "get_width");

	ADD_GROUP("BiDi", "");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "text_direction", PROPERTY_HINT_ENUM, "Auto,Left-to-Right,Right-to-Left"), "set_text_direction", "get_text_direction");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "language", PROPERTY_HINT_LOCALE_ID, ""), "set_language", "get_language");

	ADD_GROUP("Appearance", "");
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "modulate"), "set_modulate", "get_modulate");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "render_priority"), "set_render_priority", "get_render_priority");

	ADD_SIGNAL(MethodInfo("text_mesh_update"));
}

void FastText::_validate_property(PropertyInfo &p_property) const {
	if (
			p_property.name == "material_override" ||
			p_property.name == "material_overlay" ||
			p_property.name == "lod_bias" ||
			p_property.name == "gi_mode" ||
			p_property.name == "gi_lightmap_scale") {
		p_property.usage = PROPERTY_USAGE_NO_EDITOR;
	}

	if (p_property.name == "cast_shadow") {
		p_property.usage = PROPERTY_USAGE_NO_EDITOR;
	}
}

void FastText::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_TRANSLATION_CHANGED: {
			String new_text = atr(text);
			if (new_text == xl_text) {
				return; // Nothing new.
			}
			xl_text = new_text;
			dirty_text = true;
			_queue_update();
		} break;
	}
}

void FastText::_im_update() {
	DEV_ASSERT(ft_share.is_valid()); // Ensure shared resource is valid
	if (!ft_share.is_valid()) {
		return;
	}

	debug_tri_mesh.unref();

	bool updated = ft_share->update_mesh(this);
	DEV_ASSERT(updated);
	(void)updated;

	update_gizmos();

	pending_update = false;
	if (is_inside_tree()) {
		emit_signal(SNAME("text_mesh_update"));
	}
}

void FastText::_queue_update() {
	if (pending_update) {
		return;
	}

	// Sometimes this is called before its valid return and try later
	if (!ft_share.is_valid()) {
		return;
	}

	pending_update = true;
	callable_mp(this, &FastText::_im_update).call_deferred();
}

void FastText::set_text(const String &p_string) {
	if (text == p_string) {
		return;
	}

	text = p_string;
	xl_text = atr(p_string);
	dirty_text = true;
	_queue_update();
}

String FastText::get_text() const {
	return text;
}

void FastText::set_horizontal_alignment(HorizontalAlignment p_alignment) {
	ERR_FAIL_INDEX((int)p_alignment, 4);
	if (horizontal_alignment != p_alignment) {
		horizontal_alignment = p_alignment;
		_queue_update();
	}
}

HorizontalAlignment FastText::get_horizontal_alignment() const {
	return horizontal_alignment;
}

void FastText::set_vertical_alignment(VerticalAlignment p_alignment) {
	ERR_FAIL_INDEX((int)p_alignment, 4);
	if (vertical_alignment != p_alignment) {
		vertical_alignment = p_alignment;
		_queue_update();
	}
}

VerticalAlignment FastText::get_vertical_alignment() const {
	return vertical_alignment;
}

void FastText::set_render_priority(int p_priority) {
	if (render_priority == p_priority) {
		return;
	}

	render_priority = p_priority;
	_queue_update();
}

int FastText::get_render_priority() const {
	return render_priority;
}

void FastText::set_modulate(const Color &p_color) {
	if (modulate == p_color) {
		return;
	}

	modulate = p_color;

	// Set shader parameter for material
	set_instance_shader_parameter("modulate", modulate);

	_queue_update();
}

Color FastText::get_modulate() const {
	return modulate;
}

void FastText::set_text_direction(TextServer::Direction p_text_direction) {
	ERR_FAIL_COND((int)p_text_direction < -1 || (int)p_text_direction > 3);
	if (text_direction != p_text_direction) {
		text_direction = p_text_direction;
		dirty_text = true;
		_queue_update();
	}
}

TextServer::Direction FastText::get_text_direction() const {
	return text_direction;
}

void FastText::set_language(const String &p_language) {
	if (language != p_language) {
		language = p_language;
		dirty_text = true;
		_queue_update();
	}
}

String FastText::get_language() const {
	return language;
}

void FastText::set_uppercase(bool p_uppercase) {
	if (uppercase != p_uppercase) {
		uppercase = p_uppercase;
		dirty_text = true;
		_queue_update();
	}
}

bool FastText::is_uppercase() const {
	return uppercase;
}

void FastText::set_autowrap_mode(TextServer::AutowrapMode p_mode) {
	if (autowrap_mode != p_mode) {
		autowrap_mode = p_mode;
		_queue_update();
	}
}

TextServer::AutowrapMode FastText::get_autowrap_mode() const {
	return autowrap_mode;
}

void FastText::set_justification_flags(BitField<TextServer::JustificationFlag> p_flags) {
	if (jst_flags != p_flags) {
		jst_flags = p_flags;
		_queue_update();
	}
}

BitField<TextServer::JustificationFlag> FastText::get_justification_flags() const {
	return jst_flags;
}

void FastText::set_width(float p_width) {
	if (width != p_width) {
		width = p_width;
		_queue_update();
	}
}

float FastText::get_width() const {
	return width;
}

void FastText::set_offset(const Point2 &p_offset) {
	if (lbl_offset != p_offset) {
		lbl_offset = p_offset;
		_queue_update();
	}
}

Point2 FastText::get_offset() const {
	return lbl_offset;
}

void FastText::set_line_spacing(float p_line_spacing) {
	if (line_spacing != p_line_spacing) {
		line_spacing = p_line_spacing;
		_queue_update();
	}
}

float FastText::get_line_spacing() const {
	return line_spacing;
}

void FastText::set_font_shared(Ref<FastTextSharedResource> ref) {
	if (ref == ft_share) {
		return;
	}

	if (ft_share.is_valid()) {
		ft_share->remove_text(this);
	}

	ft_share = ref;
	_queue_update();
}

Ref<FastTextSharedResource> FastText::get_font_shared() const {
	return ft_share;
}

AABB FastText::get_aabb() const {
	return aabb;
}

Ref<TriangleMesh> FastText::get_triangle_mesh() const {
	if (debug_tri_mesh.is_valid()) {
		return debug_tri_mesh;
	}

	Vector<Vector3> faces = {

		// Front face
		Vector3(aabb.position.x, aabb.position.y, aabb.position.z),
		Vector3(aabb.position.x + aabb.size.x, aabb.position.y, aabb.position.z),
		Vector3(aabb.position.x, aabb.position.y + aabb.size.y, aabb.position.z),
		Vector3(aabb.position.x + aabb.size.x, aabb.position.y, aabb.position.z),
		Vector3(aabb.position.x + aabb.size.x, aabb.position.y + aabb.size.y, aabb.position.z),
		Vector3(aabb.position.x, aabb.position.y + aabb.size.y, aabb.position.z),

		// Back face
		Vector3(aabb.position.x, aabb.position.y, aabb.position.z + aabb.size.z),
		Vector3(aabb.position.x, aabb.position.y + aabb.size.y, aabb.position.z + aabb.size.z),
		Vector3(aabb.position.x + aabb.size.x, aabb.position.y, aabb.position.z + aabb.size.z),
		Vector3(aabb.position.x + aabb.size.x, aabb.position.y, aabb.position.z + aabb.size.z),
		Vector3(aabb.position.x, aabb.position.y + aabb.size.y, aabb.position.z + aabb.size.z),
		Vector3(aabb.position.x + aabb.size.x, aabb.position.y + aabb.size.y, aabb.position.z + aabb.size.z),

		// Top face
		Vector3(aabb.position.x, aabb.position.y + aabb.size.y, aabb.position.z),
		Vector3(aabb.position.x + aabb.size.x, aabb.position.y + aabb.size.y, aabb.position.z),
		Vector3(aabb.position.x, aabb.position.y + aabb.size.y, aabb.position.z + aabb.size.z),
		Vector3(aabb.position.x + aabb.size.x, aabb.position.y + aabb.size.y, aabb.position.z),
		Vector3(aabb.position.x + aabb.size.x, aabb.position.y + aabb.size.y, aabb.position.z + aabb.size.z),
		Vector3(aabb.position.x, aabb.position.y + aabb.size.y, aabb.position.z + aabb.size.z),

		// Bottom face
		Vector3(aabb.position.x, aabb.position.y, aabb.position.z),
		Vector3(aabb.position.x, aabb.position.y, aabb.position.z + aabb.size.z),
		Vector3(aabb.position.x + aabb.size.x, aabb.position.y, aabb.position.z),
		Vector3(aabb.position.x + aabb.size.x, aabb.position.y, aabb.position.z),
		Vector3(aabb.position.x, aabb.position.y, aabb.position.z + aabb.size.z),
		Vector3(aabb.position.x + aabb.size.x, aabb.position.y, aabb.position.z + aabb.size.z),

		// Left face
		Vector3(aabb.position.x, aabb.position.y, aabb.position.z),
		Vector3(aabb.position.x, aabb.position.y + aabb.size.y, aabb.position.z),
		Vector3(aabb.position.x, aabb.position.y, aabb.position.z + aabb.size.z),
		Vector3(aabb.position.x, aabb.position.y + aabb.size.y, aabb.position.z),
		Vector3(aabb.position.x, aabb.position.y + aabb.size.y, aabb.position.z + aabb.size.z),
		Vector3(aabb.position.x, aabb.position.y, aabb.position.z + aabb.size.z),

		// Right face
		Vector3(aabb.position.x + aabb.size.x, aabb.position.y, aabb.position.z),
		Vector3(aabb.position.x + aabb.size.x, aabb.position.y, aabb.position.z + aabb.size.z),
		Vector3(aabb.position.x + aabb.size.x, aabb.position.y + aabb.size.y, aabb.position.z),
		Vector3(aabb.position.x + aabb.size.x, aabb.position.y + aabb.size.y, aabb.position.z),
		Vector3(aabb.position.x + aabb.size.x, aabb.position.y, aabb.position.z + aabb.size.z),
		Vector3(aabb.position.x + aabb.size.x, aabb.position.y + aabb.size.y, aabb.position.z + aabb.size.z)
	};

	debug_tri_mesh.instantiate();
	debug_tri_mesh->create(faces);
	return debug_tri_mesh;
}

FastText::FastText() {
	text_rid = TS->create_shaped_text();
	DEV_ASSERT(text_rid.is_valid()); // Ensure text_rid is valid

	mesh = RenderingServer::get_singleton()->mesh_create();
	DEV_ASSERT(mesh.is_valid()); // Ensure mesh is valid

	// Initialize color
	set_modulate(modulate);

	// Disable shadow casting by default to improve performance and avoid unintended visual artifacts.
	set_cast_shadows_setting(SHADOW_CASTING_SETTING_OFF);

	// FastText can't contribute to GI in any way, so disable it to improve performance.
	set_gi_mode(GI_MODE_DISABLED);

	set_base(mesh);
}

FastText::~FastText() {
	if (ft_share.is_valid()) {
		ft_share->remove_text(this);
	}

	for (int i = 0; i < lines_rid.size(); i++) {
		TS->free_rid(lines_rid[i]);
	}
	lines_rid.clear();

	TS->free_rid(text_rid);

	ERR_FAIL_NULL(RenderingServer::get_singleton());
	RenderingServer::get_singleton()->free(mesh);
}
