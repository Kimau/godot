#include "fast_text.h"
#include "fast_text_shared.h"

void FastTextSharedResource::_validate_property(PropertyInfo &p_property) const {
    if (
            p_property.name == "material_overlay" ||
            p_property.name == "lod_bias" ||
            p_property.name == "gi_mode" ||
            p_property.name == "gi_lightmap_scale") {
        p_property.usage = PROPERTY_USAGE_NO_EDITOR;
    }

    /*
    if (p_property.name == "cast_shadow" && alpha_cut == ALPHA_CUT_DISABLED) {
        // Alpha-blended materials can't cast shadows.
        p_property.usage = PROPERTY_USAGE_NO_EDITOR;
    }
*/
}

void FastTextSharedResource::_bind_methods() {
    // Bind all methods
	ClassDB::bind_method(D_METHOD("set_font", "font"), &FastTextSharedResource::set_font);
	ClassDB::bind_method(D_METHOD("get_font"), &FastTextSharedResource::get_font);
	ClassDB::bind_method(D_METHOD("set_pixel_size", "size"), &FastTextSharedResource::set_pixel_size);
	ClassDB::bind_method(D_METHOD("get_pixel_size"), &FastTextSharedResource::get_pixel_size);
	ClassDB::bind_method(D_METHOD("set_font_size", "size"), &FastTextSharedResource::set_font_size);
	ClassDB::bind_method(D_METHOD("get_font_size"), &FastTextSharedResource::get_font_size);
	ClassDB::bind_method(D_METHOD("set_alpha_cut_mode", "mode"), &FastTextSharedResource::set_alpha_cut_mode);
	ClassDB::bind_method(D_METHOD("get_alpha_cut_mode"), &FastTextSharedResource::get_alpha_cut_mode);
    ClassDB::bind_method(D_METHOD("set_draw_flag", "flag", "enabled"), &FastTextSharedResource::draw_flag_set);
	ClassDB::bind_method(D_METHOD("get_draw_flag", "flag"), &FastTextSharedResource::draw_flag_get);
    ClassDB::bind_method(D_METHOD("set_material_override", "material"), &FastTextSharedResource::set_material_override);
    ClassDB::bind_method(D_METHOD("get_material_override"), &FastTextSharedResource::get_material_override);

	// Add property groups and properties
	ADD_GROUP("Font", "");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "font", PROPERTY_HINT_RESOURCE_TYPE, "Font"), "set_font", "get_font");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "font_size", PROPERTY_HINT_RANGE, "1,256,1,or_greater,suffix:px"), "set_font_size", "get_font_size");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "pixel_size", PROPERTY_HINT_RANGE, "0.0001,128,0.0001,suffix:m"), "set_pixel_size", "get_pixel_size");

	ADD_GROUP("Alpha", "");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "alpha_cut", PROPERTY_HINT_ENUM, "Disabled,Discard,Opaque Pre-Pass,Alpha Hash"), "set_alpha_cut_mode", "get_alpha_cut_mode");

	ADD_GROUP("Flags", "");
	ADD_PROPERTYI(PropertyInfo(Variant::BOOL, "shaded"), "set_draw_flag", "get_draw_flag", FLAG_SHADED);
	ADD_PROPERTYI(PropertyInfo(Variant::BOOL, "double_sided"), "set_draw_flag", "get_draw_flag", FLAG_DOUBLE_SIDED);
	ADD_PROPERTYI(PropertyInfo(Variant::BOOL, "no_depth_test"), "set_draw_flag", "get_draw_flag", FLAG_DISABLE_DEPTH_TEST);
	ADD_PROPERTYI(PropertyInfo(Variant::BOOL, "fixed_size"), "set_draw_flag", "get_draw_flag", FLAG_FIXED_SIZE);

    ADD_GROUP("Material", "");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "material_override", PROPERTY_HINT_RESOURCE_TYPE, "ShaderMaterial"), "set_material_override", "get_material_override");

	// Enums
    BIND_ENUM_CONSTANT(ALPHA_CUT_DISABLED);
	BIND_ENUM_CONSTANT(ALPHA_CUT_DISCARD);
	BIND_ENUM_CONSTANT(ALPHA_CUT_OPAQUE_PREPASS);
	BIND_ENUM_CONSTANT(ALPHA_CUT_HASH);
	BIND_ENUM_CONSTANT(ALPHA_CUT_MAX);

    BIND_ENUM_CONSTANT(FLAG_SHADED);
	BIND_ENUM_CONSTANT(FLAG_DOUBLE_SIDED);
	BIND_ENUM_CONSTANT(FLAG_DISABLE_DEPTH_TEST);
	BIND_ENUM_CONSTANT(FLAG_FIXED_SIZE);
	BIND_ENUM_CONSTANT(FLAG_MAX);
}


RID create_font_mat(const FastTextSharedResource::MatParams &params, const Ref<ShaderMaterial> &p_material_override) {
    RID font_mat;

    // Use custom shader material if available
    if (p_material_override.is_valid()) {
        font_mat = p_material_override->get_rid();

        // Set standard font texture
        RS::get_singleton()->material_set_param(font_mat, "texture_albedo", params.tex_id);

        // Set MSDF parameters if needed
        if (params.msdf) {
            RS::get_singleton()->material_set_param(font_mat, "msdf_pixel_range", TS->font_get_msdf_pixel_range(params.font_id));
            RS::get_singleton()->material_set_param(font_mat, "msdf_outline_size", 0.0);
        }

        return font_mat;
    }

    // Otherwise create a standard material
	font_mat = RenderingServer::get_singleton()->material_create();

	const float scissor_threshold = 0.5f;
	const float alpha_hash_scale = 1.0f;
	const float alpha_antialiasing_edge = 0.0f;

	RS::get_singleton()->material_set_param(font_mat, "albedo", Color(1, 1, 1, 1));
	RS::get_singleton()->material_set_param(font_mat, "specular", 0.5);
	RS::get_singleton()->material_set_param(font_mat, "metallic", 0.0);
	RS::get_singleton()->material_set_param(font_mat, "roughness", 1.0);
	RS::get_singleton()->material_set_param(font_mat, "uv1_offset", Vector3(0, 0, 0));
	RS::get_singleton()->material_set_param(font_mat, "uv1_scale", Vector3(1, 1, 1));
	RS::get_singleton()->material_set_param(font_mat, "uv2_offset", Vector3(0, 0, 0));
	RS::get_singleton()->material_set_param(font_mat, "uv2_scale", Vector3(1, 1, 1));
	RS::get_singleton()->material_set_param(font_mat, "alpha_scissor_threshold", scissor_threshold);
	RS::get_singleton()->material_set_param(font_mat, "alpha_hash_scale", alpha_hash_scale);
	RS::get_singleton()->material_set_param(font_mat, "alpha_antialiasing_edge", alpha_antialiasing_edge);

	if (params.msdf) {
		RS::get_singleton()->material_set_param(font_mat, "msdf_pixel_range", TS->font_get_msdf_pixel_range(params.font_id));
		RS::get_singleton()->material_set_param(font_mat, "msdf_outline_size", 0.0);
	}

	BaseMaterial3D::Transparency mat_transparency;
	switch (params.flag_alpha) {
		case FastTextSharedResource::ALPHA_CUT_DISCARD:
			mat_transparency = BaseMaterial3D::Transparency::TRANSPARENCY_ALPHA_SCISSOR;
			break;
		case FastTextSharedResource::ALPHA_CUT_OPAQUE_PREPASS:
			mat_transparency = BaseMaterial3D::Transparency::TRANSPARENCY_ALPHA_DEPTH_PRE_PASS;
			break;
		case FastTextSharedResource::ALPHA_CUT_HASH:
			mat_transparency = BaseMaterial3D::Transparency::TRANSPARENCY_ALPHA_HASH;
			break;
		case FastTextSharedResource::ALPHA_CUT_DISABLED:
		default:
			mat_transparency = BaseMaterial3D::Transparency::TRANSPARENCY_ALPHA;
			break;
	}

	const bool is_shaded = params.flag_draw & FastTextSharedResource::FLAG_SHADED;
	const bool is_double_sided = params.flag_draw & FastTextSharedResource::FLAG_DOUBLE_SIDED;
	const bool disable_depth_test = params.flag_draw & FastTextSharedResource::FLAG_DISABLE_DEPTH_TEST;
	const bool is_fixed_size = params.flag_draw & FastTextSharedResource::FLAG_FIXED_SIZE;
	const bool billboard_mode = StandardMaterial3D::BILLBOARD_DISABLED;
	const StandardMaterial3D::TextureFilter texture_filter = StandardMaterial3D::TEXTURE_FILTER_LINEAR_WITH_MIPMAPS;
	const StandardMaterial3D::AlphaAntiAliasing alpha_antialiasing_mode = StandardMaterial3D::ALPHA_ANTIALIASING_OFF;

	RID shader_rid;
	StandardMaterial3D::get_material_for_2d(
			is_shaded, mat_transparency,
			is_double_sided, billboard_mode == StandardMaterial3D::BILLBOARD_ENABLED,
			billboard_mode == StandardMaterial3D::BILLBOARD_FIXED_Y, params.msdf, disable_depth_test,
			is_fixed_size, texture_filter, alpha_antialiasing_mode, &shader_rid);

	RS::get_singleton()->material_set_shader(font_mat, shader_rid);
	RS::get_singleton()->material_set_param(font_mat, "texture_albedo", params.tex_id);

	if (params.flag_alpha == FastTextSharedResource::ALPHA_CUT_DISABLED)
		RS::get_singleton()->material_set_render_priority(font_mat, 0);

	return font_mat;
}


RID FastTextSharedResource::get_create_mat(RID font_id, uint32_t glyph_index, Vector2i fsize, bool msdf,
		AlphaCutMode flag_alpha, int32_t flag_draw) {
	RID font_tex = TS->font_get_glyph_texture_rid(font_id, fsize, glyph_index);

	MatParams params = {
		font_id, font_tex, fsize, msdf, flag_alpha, flag_draw
	};

	// Always check cache first, regardless of material override
	if (mat_cache.has(params)) {
		return mat_cache.get(params);
	}

	// Create and cache the material (with or without override)
	RID mat = create_font_mat(params, material_override);
	mat_cache.insert(params, mat);
	return mat;
}


void FastTextSharedResource::_notify_change() {
	for (FastText *text : users)
		text->_queue_update();
}

void FastTextSharedResource::remove_text(FastText* text) {
	users.erase(text);
}

// Text API -- Updates the Mesh data of FT
bool FastTextSharedResource::update_mesh(FastText *text) {
	DEV_ASSERT(text != nullptr); // Ensure text pointer is valid
	if (!text)
		return false;

	// When a shaped text is invalidated by an external source, we want to reshape it.
	if (!TS->shaped_text_is_ready(text->text_rid)) {
		text->dirty_text = true;
	}

	if (!users.has(text))
		users.push_back(text);

	DEV_ASSERT(font.is_valid()); // Ensure font is valid
	if (!font.is_valid())
		return false;

	// Update text buffer.
	if (text->dirty_text) {
		TS->shaped_text_clear(text->text_rid);
		TS->shaped_text_set_direction(text->text_rid, text->text_direction);

		String txt = (text->uppercase) ? TS->string_to_upper(text->xl_text, text->language) : text->xl_text;
		TS->shaped_text_add_string(text->text_rid, txt, font->get_rids(), font_size, font->get_opentype_features(), text->language);

		TypedArray<Vector3i> stt = TS->parse_structured_text(TextServer::STRUCTURED_TEXT_DEFAULT, {}, txt);
		TS->shaped_text_set_bidi_override(text->text_rid, stt);

		text->dirty_text = false;
	} else { // Font is always dirty
		int spans = TS->shaped_get_span_count(text->text_rid);
		for (int i = 0; i < spans; i++) {
			TS->shaped_set_span_update_font(text->text_rid, i, font->get_rids(), font_size, font->get_opentype_features());
		}
	}

	// Dirty Lines
	{
		for (int i = 0; i < text->lines_rid.size(); i++) {
			TS->free_rid(text->lines_rid[i]);
		}
		text->lines_rid.clear();

		BitField<TextServer::LineBreakFlag> autowrap_flags = TextServer::BREAK_MANDATORY;
		switch (text->autowrap_mode) {
			case TextServer::AUTOWRAP_WORD_SMART:
				autowrap_flags = TextServer::BREAK_WORD_BOUND | TextServer::BREAK_ADAPTIVE | TextServer::BREAK_MANDATORY;
				break;
			case TextServer::AUTOWRAP_WORD:
				autowrap_flags = TextServer::BREAK_WORD_BOUND | TextServer::BREAK_MANDATORY;
				break;
			case TextServer::AUTOWRAP_ARBITRARY:
				autowrap_flags = TextServer::BREAK_GRAPHEME_BOUND | TextServer::BREAK_MANDATORY;
				break;
			case TextServer::AUTOWRAP_OFF:
				break;
		}
		autowrap_flags = autowrap_flags | TextServer::BREAK_TRIM_EDGE_SPACES;

		PackedInt32Array line_breaks = TS->shaped_text_get_line_breaks(text->text_rid, text->width, 0, autowrap_flags);
		float max_line_w = 0.0;
		for (int i = 0; i < line_breaks.size(); i = i + 2) {
			RID line = TS->shaped_text_substr(text->text_rid, line_breaks[i], line_breaks[i + 1] - line_breaks[i]);
			max_line_w = MAX(max_line_w, TS->shaped_text_get_width(line));
			text->lines_rid.push_back(line);
		}

		if (text->horizontal_alignment == HORIZONTAL_ALIGNMENT_FILL) {
			int jst_to_line = text->lines_rid.size();
			if (text->lines_rid.size() == 1 && text->jst_flags.has_flag(TextServer::JUSTIFICATION_DO_NOT_SKIP_SINGLE_LINE)) {
				jst_to_line = text->lines_rid.size();
			} else {
				if (text->jst_flags.has_flag(TextServer::JUSTIFICATION_SKIP_LAST_LINE)) {
					jst_to_line = text->lines_rid.size() - 1;
				}
				if (text->jst_flags.has_flag(TextServer::JUSTIFICATION_SKIP_LAST_LINE_WITH_VISIBLE_CHARS)) {
					for (int i = text->lines_rid.size() - 1; i >= 0; i--) {
						if (TS->shaped_text_has_visible_chars(text->lines_rid[i])) {
							jst_to_line = i;
							break;
						}
					}
				}
			}
			for (int i = 0; i < jst_to_line; i++) {
				TS->shaped_text_fit_to_width(text->lines_rid[i], (text->width > 0) ? text->width : max_line_w, text->jst_flags);
			}
		}
	}

	// Generate surfaces and materials.
	float total_h = 0.0;
	for (int i = 0; i < text->lines_rid.size(); i++) {
		total_h += (TS->shaped_text_get_size(text->lines_rid[i]).y + text->line_spacing) * pixel_size;
	}

	float vbegin = 0.0;
	switch (text->vertical_alignment) {
		case VERTICAL_ALIGNMENT_FILL:
		case VERTICAL_ALIGNMENT_TOP: {
			// Nothing.
		} break;
		case VERTICAL_ALIGNMENT_CENTER: {
			vbegin = (total_h - text->line_spacing * pixel_size) / 2.0;
		} break;
		case VERTICAL_ALIGNMENT_BOTTOM: {
			vbegin = (total_h - text->line_spacing * pixel_size);
		} break;
	}

	AABB newAABB;
	Vector2 cursor = Vector2(0, vbegin + text->lbl_offset.y * pixel_size);

	float z_shift = 0.0f;

	struct SurfaceData {
		PackedVector3Array mesh_vertices;
		PackedVector2Array mesh_uvs;
		PackedInt32Array indices;
		int offset = 0;
		RID material;
		RID texture;
	};
	LocalVector<SurfaceData*> surfs;

	// for EACH LINE
	for (int line_idx = 0; line_idx < text->lines_rid.size(); line_idx++) {
		const Glyph *glyphs = TS->shaped_text_get_glyphs(text->lines_rid[line_idx]);
		int gl_size = TS->shaped_text_get_glyph_count(text->lines_rid[line_idx]);
		float line_width = TS->shaped_text_get_width(text->lines_rid[line_idx]) * pixel_size;

		switch (text->horizontal_alignment) {
			case HORIZONTAL_ALIGNMENT_LEFT:
				cursor.x = 0.0;
			break;
			case HORIZONTAL_ALIGNMENT_FILL:
			case HORIZONTAL_ALIGNMENT_CENTER: {
				cursor.x = -line_width / 2.0;
			} break;
			case HORIZONTAL_ALIGNMENT_RIGHT: {
				cursor.x = -line_width;
			} break;
		}
		cursor.x += text->lbl_offset.x * pixel_size;
		if (newAABB == AABB()) {
			newAABB.position = Vector3(cursor.x, cursor.y, 0);
			newAABB.expand_to(Vector3(cursor.x + line_width, cursor.y - (TS->shaped_text_get_size(text->lines_rid[line_idx]).y + text->line_spacing) * pixel_size, 0));
		} else {
			newAABB.expand_to(Vector3(cursor.x, cursor.y, 0));
			newAABB.expand_to(Vector3(cursor.x + line_width, cursor.y - (TS->shaped_text_get_size(text->lines_rid[line_idx]).y + text->line_spacing) * pixel_size, 0));
		}

		cursor.y -= TS->shaped_text_get_ascent(text->lines_rid[line_idx]) * pixel_size;

		// for EACH GLYPH in line
		for (int glyph_idx = 0; glyph_idx < gl_size; glyph_idx++) {
			const Glyph& g = glyphs[glyph_idx];
			float step_x = g.advance * pixel_size * g.repeat;

			// Non Printable
			if ((g.index == 0) || (!g.font_rid.is_valid())) {
				cursor.x += step_x;
				continue;
			}

			Vector2i fsize = Vector2i(g.font_size, 0);
			RID texture_rid = TS->font_get_glyph_texture_rid(g.font_rid, fsize, g.index);

			// find index of surface
			if (!texture_rid.is_valid()) {
				cursor.x += step_x;
				continue;
			}


			Vector2 gl_of = (TS->font_get_glyph_offset(g.font_rid, fsize, g.index) + Vector2(g.x_off, g.y_off)) * pixel_size;
			Vector2 gl_sz = TS->font_get_glyph_size(g.font_rid, fsize, g.index) * pixel_size;
			Rect2 gl_uv = TS->font_get_glyph_uv_rect(g.font_rid, fsize, g.index);
			Size2 tex_size = TS->font_get_glyph_texture_size(g.font_rid, fsize, g.index);

			// too small to draw
			if (gl_uv.size.x <= 2 || gl_uv.size.y <= 2) {
				cursor.x += step_x;
				continue;
			}

			// Find surface if one exist
			int i=0;
			RID mat_id = get_create_mat(g.font_rid, g.index, fsize, TS->font_is_multichannel_signed_distance_field(g.font_rid), alpha_cut, font_draw_flags);

			SurfaceData* s = nullptr;
			while (i<surfs.size()) {
				// Why check both instancing bug - need to double check later
				if ((surfs[i]->texture == texture_rid) && (surfs[i]->material == mat_id)) {
					s = surfs[i];
					break;
				}
				i++;
			}

			if (s == nullptr) {
				s = new SurfaceData();
				s->texture = texture_rid;
				s->material = mat_id;
				surfs.push_back(s);
			}

			// for EACH REPEAT
			for (int j = 0; j < g.repeat; j++) {

				int sidx = s->offset;
				s->offset += 1;

				s->mesh_vertices.append_array({
					Vector3(cursor.x + gl_of.x, cursor.y - gl_of.y, z_shift),
					Vector3(cursor.x + gl_of.x + gl_sz.x, cursor.y - gl_of.y, z_shift),
					Vector3(cursor.x + gl_of.x + gl_sz.x, cursor.y - gl_of.y - gl_sz.y, z_shift),
					Vector3(cursor.x + gl_of.x, cursor.y - gl_of.y - gl_sz.y, z_shift)
				});

				// to offset the border
				gl_uv.position += Vector2i(1,1);
				gl_uv.size -= Vector2i(2,2);

				s->mesh_uvs.append_array({
				Vector2(gl_uv.position.x / tex_size.x, gl_uv.position.y / tex_size.y),
				Vector2((gl_uv.position.x + gl_uv.size.x) / tex_size.x, gl_uv.position.y / tex_size.y),
				Vector2((gl_uv.position.x + gl_uv.size.x) / tex_size.x, (gl_uv.position.y + gl_uv.size.y) / tex_size.y),
				Vector2(gl_uv.position.x / tex_size.x, (gl_uv.position.y + gl_uv.size.y) / tex_size.y)
					});

				s->indices.append_array({
				(sidx * 4) + 0,
				(sidx * 4) + 1,
				(sidx * 4) + 2,
				(sidx * 4) + 0,
				(sidx * 4) + 2,
				(sidx * 4) + 3
				});

				cursor.x += g.advance * pixel_size;
			} // end repeat
		} // end glyph

		cursor.y -= (TS->shaped_text_get_descent(text->lines_rid[line_idx]) + text->line_spacing) * pixel_size;
	} // end line

	// HACK :: THERE IS KNOWN BUG ON ANDROID WHERE THE INSTANCE UNIFORMS ARE NUKED
	// Can't recall what this about, you fucking idiot Claire write better comments
	// Leaving here for future facepalm

	// Clear the mesh
	RS::get_singleton()->mesh_clear(text->mesh);

	for (const SurfaceData* s : surfs) {
		Array mesh_array;
		mesh_array.resize(RS::ARRAY_MAX);
		mesh_array[RS::ARRAY_VERTEX] = s->mesh_vertices;

		// Use instance shader parameter for colors instead of vertex colors
		// Why waste all that buffer space
		// Note this requires using a custom shader

		mesh_array[RS::ARRAY_TEX_UV] = s->mesh_uvs;
		mesh_array[RS::ARRAY_INDEX] = s->indices;

		RS::SurfaceData sd;
		RS::get_singleton()->mesh_create_surface_data_from_arrays(&sd, RS::PRIMITIVE_TRIANGLES, mesh_array);

		sd.material = s->material;

		RS::get_singleton()->mesh_add_surface(text->mesh, sd);
	}

	text->aabb = newAABB;

	return true;
}


void FastTextSharedResource::set_font(const Ref<Font>& p_font) {
	if (font == p_font)
		return;

	font = p_font;
	_notify_change();
}

Ref<Font> FastTextSharedResource::get_font() const {
	return font;
}

void FastTextSharedResource::set_font_size(int p_size) {
	if (p_size == font_size)
		return;

	font_size = p_size;
	_notify_change();
}

void FastTextSharedResource::set_pixel_size(real_t p_size) {
	if (pixel_size == p_size)
		return;

	pixel_size = p_size;
	_notify_change();
}

real_t FastTextSharedResource::get_pixel_size() const {
	return pixel_size;
}

int FastTextSharedResource::get_font_size() const {
	return font_size;
}

void FastTextSharedResource::set_alpha_cut_mode(FastTextSharedResource::AlphaCutMode new_alpha_mode) {
	if (alpha_cut == new_alpha_mode)
		return;

	alpha_cut = new_alpha_mode;
	_notify_change();
}

FastTextSharedResource::AlphaCutMode FastTextSharedResource::get_alpha_cut_mode() const {
	return alpha_cut;
}

void FastTextSharedResource::set_material_override(const Ref<ShaderMaterial> &p_material) {
    if (material_override == p_material)
        return;

    material_override = p_material;

    // Clear material cache when material changes
    for (auto &kv : mat_cache) {
        RenderingServer::get_singleton()->free(kv.value);
    }
    mat_cache.clear();

    _notify_change();
}

Ref<ShaderMaterial> FastTextSharedResource::get_material_override() const {
    return material_override;
}

bool FastTextSharedResource::draw_flag_get(DrawFlags df) const {
	return font_draw_flags & df;
}


void FastTextSharedResource::draw_flag_set(DrawFlags df, bool enable) {
	if (draw_flag_get(df) == enable)
		return;

	if (enable)
		font_draw_flags |= df;
	else
		font_draw_flags &= ~df;

	_notify_change();
}

FastTextSharedResource::FastTextSharedResource() {
	draw_flag_set(FLAG_DOUBLE_SIDED, true);
}

FastTextSharedResource::~FastTextSharedResource() {
	for (auto &kv : mat_cache) {
		RenderingServer::get_singleton()->free(kv.value);
	}
}
