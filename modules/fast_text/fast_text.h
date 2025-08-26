/* fast_text.h - 3D text rendering component */
#ifndef FAST_TEXT_H
#define FAST_TEXT_H

#include "fast_text_shared.h"
#include "scene/3d/visual_instance_3d.h"
#include "scene/resources/font.h"
#include "servers/text_server.h"

class FastText : public GeometryInstance3D {
	friend class FastTextSharedResource;
	GDCLASS(FastText, GeometryInstance3D);

private:
	Ref<FastTextSharedResource> ft_share;
	mutable Ref<TriangleMesh> debug_tri_mesh; // For gizmo

	RID mesh;
	HorizontalAlignment horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER;
	VerticalAlignment vertical_alignment = VERTICAL_ALIGNMENT_CENTER;
	String text;
	String xl_text;
	bool uppercase = false;

	TextServer::AutowrapMode autowrap_mode = TextServer::AUTOWRAP_OFF;
	BitField<TextServer::JustificationFlag> jst_flags = TextServer::JUSTIFICATION_WORD_BOUND | TextServer::JUSTIFICATION_KASHIDA | TextServer::JUSTIFICATION_SKIP_LAST_LINE | TextServer::JUSTIFICATION_DO_NOT_SKIP_SINGLE_LINE;
	float width = 500.0;

	Point2 lbl_offset;
	AABB aabb;

	float line_spacing = 0.f;

	String language;
	TextServer::Direction text_direction = TextServer::DIRECTION_AUTO;

	RID text_rid;
	Vector<RID> lines_rid;

	bool pending_update = false;
	bool dirty_text = true;

	// Render priority settings
	int render_priority = 0;

	// Text color
	Color modulate = Color(1, 1, 1, 1);

protected:
	GDVIRTUAL2RC(TypedArray<Vector3i>, _structured_text_parser, Array, String)

	void _notification(int p_what);
	static void _bind_methods();
	void _validate_property(PropertyInfo &p_property) const;
	void _im_update();
	void _queue_update();

public:
	void set_horizontal_alignment(HorizontalAlignment p_alignment); // Set alignment (Left, Center, Right, Fill)
	HorizontalAlignment get_horizontal_alignment() const;

	void set_vertical_alignment(VerticalAlignment p_alignment); // Set alignment (Top, Center, Bottom)
	VerticalAlignment get_vertical_alignment() const;

	void set_render_priority(int p_priority); // Higher values render on top
	int get_render_priority() const;

	void set_text(const String &p_string); // Text to display
	String get_text() const;

	void set_text_direction(TextServer::Direction p_text_direction); // BiDi text direction
	TextServer::Direction get_text_direction() const;

	void set_language(const String &p_language); // Text language identifier
	String get_language() const;

	void set_uppercase(bool p_uppercase); // Convert to uppercase
	bool is_uppercase() const;

	void set_line_spacing(float p_size); // Space between lines (px)
	float get_line_spacing() const;

	void set_autowrap_mode(TextServer::AutowrapMode p_mode); // Text wrapping mode
	TextServer::AutowrapMode get_autowrap_mode() const;

	void set_justification_flags(BitField<TextServer::JustificationFlag> p_flags); // Justification settings
	BitField<TextServer::JustificationFlag> get_justification_flags() const;

	void set_width(float p_width); // Max width before wrapping (px)
	float get_width() const;

	void set_offset(const Point2 &p_offset); // Text position offset
	Point2 get_offset() const;

	void set_modulate(const Color &p_color); // Set text color
	Color get_modulate() const;

	void set_font_shared(Ref<FastTextSharedResource> ref); // Set shared font resource
	Ref<FastTextSharedResource> get_font_shared() const;

	Ref<TriangleMesh> get_triangle_mesh() const; // For editor gizmo

	virtual AABB get_aabb() const override; // Text bounding box

	FastText();
	~FastText();
};

#endif // FAST_TEXT_H
