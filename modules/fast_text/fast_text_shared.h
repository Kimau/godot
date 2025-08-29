/* fast_text_shared.h - Shared resource for 3D text rendering */
#ifndef FAST_TEXT_SHARED_H
#define FAST_TEXT_SHARED_H

#include "core/io/resource.h"
#include "scene/resources/font.h"
#include "scene/resources/material.h"
#include "scene/3d/visual_instance_3d.h"
#include "servers/text_server.h"

class FastText;

class FastTextSharedResource : public Resource {
	GDCLASS(FastTextSharedResource, Resource);

public:
	enum DrawFlags {
		FLAG_SHADED = 1 << 0,         // Enable shading
		FLAG_DOUBLE_SIDED = 1 << 1,   // Render both sides of faces
		FLAG_DISABLE_DEPTH_TEST = 1 << 2, // Disable depth testing
		FLAG_FIXED_SIZE = 1 << 3,     // Use fixed size regardless of distance

		FLAG_MAX = 1 << 15
	};

	enum AlphaCutMode {
		ALPHA_CUT_DISABLED,           // Normal alpha blending
		ALPHA_CUT_DISCARD,            // Discard pixels below threshold
		ALPHA_CUT_OPAQUE_PREPASS,     // Render opaque pixels first
		ALPHA_CUT_HASH,               // Use alpha hashing for smoothing
		ALPHA_CUT_MAX
	};

	struct MatParams {
		RID font_id;                  // Font resource ID
		RID tex_id;                   // Texture resource ID
		Vector2i fsize;               // Font size
		bool msdf;                    // Is multi-channel signed distance field
		AlphaCutMode flag_alpha;      // Alpha cut mode
		int32_t flag_draw;            // Drawing flags

		bool operator==(const MatParams &other) const {
			return font_id == other.font_id &&
					tex_id == other.tex_id &&
					fsize == other.fsize &&
					msdf == other.msdf &&
					flag_alpha == other.flag_alpha &&
					flag_draw == other.flag_draw;
		}
	};

	struct MatKeyHasher {
		_FORCE_INLINE_ static uint32_t hash(const MatParams &p_a) {
			return hash_murmur3_buffer(&p_a, sizeof(MatParams));
		}
	};

private:
	struct SurfaceData {
		PackedVector3Array mesh_vertices;
		PackedVector2Array mesh_uvs;
		PackedInt32Array indices;
		int offset = 0;
		RID material;
	};

	HashMap<MatParams, RID, MatKeyHasher> mat_cache;  // Material cache

	Ref<ShaderMaterial> material_override;
	StandardMaterial3D::BillboardMode billboard_mode = StandardMaterial3D::BILLBOARD_DISABLED;
	StandardMaterial3D::TextureFilter texture_filter = StandardMaterial3D::TEXTURE_FILTER_LINEAR_WITH_MIPMAPS;

	int32_t font_draw_flags;
	real_t pixel_size = 0.005;
	FastTextSharedResource::AlphaCutMode alpha_cut = FastTextSharedResource::ALPHA_CUT_DISCARD;
	int font_size = 32;
	Ref<Font> font;

	Vector<FastText*> users;  // FastText instances using this resource

	RID get_create_mat(RID font_id, uint32_t glyph_index, Vector2i fsize, bool msdf,
			AlphaCutMode flag_alpha, int32_t flag_draw);

	void _notify_change();

protected:
	static void _bind_methods();
	void _validate_property(PropertyInfo &p_property) const;

public:
	bool update_mesh(FastText *text);  // Update mesh for a FastText instance
	void remove_text(FastText *text);  // Remove a FastText instance from users

	void set_font(const Ref<Font> &p_font);  // Set font for text rendering
	Ref<Font> get_font() const;

	void set_font_size(int p_size);  // Set font size in points
	int get_font_size() const;

	void set_pixel_size(real_t p_size);  // Set 3D scale of text
	real_t get_pixel_size() const;

	void set_alpha_cut_mode(AlphaCutMode alpha_mode);  // Set transparency handling
	AlphaCutMode get_alpha_cut_mode() const;

	void set_material_override(const Ref<ShaderMaterial> &p_material);  // Set shader override material
	Ref<ShaderMaterial> get_material_override() const;

	bool draw_flag_get(DrawFlags df) const;  // Check if draw flag is enabled
	void draw_flag_set(DrawFlags df, bool enable);  // Set a draw flag

	FastTextSharedResource();
	~FastTextSharedResource();
};

VARIANT_ENUM_CAST(FastTextSharedResource::DrawFlags);
VARIANT_ENUM_CAST(FastTextSharedResource::AlphaCutMode);

#endif // FAST_TEXT_SHARED_H
