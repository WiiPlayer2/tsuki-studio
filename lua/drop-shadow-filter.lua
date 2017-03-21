SETTING_SHADOW_COLOR = "shadow_color"
SETTING_OFFSET_X = "offset_x"
SETTING_OFFSET_Y = "offset_y"
SETTING_HIDE_IMAGE = "hide_image"
SETTING_HIDE_SHADOW = "hide_shadow"

TEXT_SHADOW_COLOR = "Shadow Color"
TEXT_OFFSET_X = "Shadow Offset X"
TEXT_OFFSET_Y = "Shadow Offset Y"
TEXT_HIDE_IMAGE = "Hide Image"
TEXT_HIDE_SHADOW = "Hide Shadow"

effect_content = [[
uniform float4x4 ViewProj;
uniform texture2d image;

uniform float4 shadow_color;
uniform float2 uv_offset;

uniform float2 uv_mul_val;
uniform float2 uv_add_val;
uniform bool hide_image;
uniform bool hide_shadow;

sampler_state textureSampler {
	Filter    = Linear;
	AddressU  = Border;
	AddressV  = Border;
	BorderColor = 00000000;
};

struct VertDataIn {
	float4 pos : POSITION;
	float2 uv  : TEXCOORD0;
};

struct VertDataOut {
	float4 pos : POSITION;
	float2 uv  : TEXCOORD0;
    float2 shadow_uv : TEXCOORD1;
};

VertDataOut VSDefault(VertDataIn v_in)
{
	VertDataOut vert_out;
	vert_out.pos = mul(float4(v_in.pos.xyz, 1.0), ViewProj);
	vert_out.uv  = v_in.uv * uv_mul_val + uv_add_val;
    vert_out.shadow_uv = v_in.uv - uv_offset;
	return vert_out;
}

float4 PassThrough(VertDataOut v_in) : TARGET
{
    float4 rgba;
    if(hide_image)
    {
        rgba = float4(0.0, 0.0, 0.0, 0.0);
    }
    else
    {
        rgba = image.Sample(textureSampler, v_in.uv);
    }
    
    float4 shadow_rgba;
    if(hide_shadow)
    {
        shadow_rgba = float4(0.0, 0.0, 0.0, 0.0);
    }
    else
    {
        float4 off_rgba = image.Sample(textureSampler, v_in.uv - uv_offset);
        shadow_rgba = shadow_color;
        shadow_rgba.a *= off_rgba.a;
    }
    
    
    return (rgba.a * rgba) + ((1.0 - rgba.a) * shadow_rgba);
}

technique Draw
{
	pass
	{
		vertex_shader = VSDefault(v_in);
		pixel_shader  = PassThrough(v_in);
	}
}
]]

function load_effect()
	local file = os.getenv("TMP") .. os.tmpname()
	obs.log(obs.LOG_DEBUG, file)
	local fh = io.open(file, "w")
	fh:write(effect_content)
	fh:flush()
	fh:close()
	local effect = gs.effect_create_from_file(file)
	os.remove(file)
	return effect
end

function calculate_sizes(filter)
	filter.render_width = filter.target_width + math.abs(filter.offset:x())
	filter.render_height = filter.target_height + math.abs(filter.offset:y())
	
	gs.vec2_set(filter.uv_mul_val, 1.0, 1.0)
	if (filter.render_width ~= 0 and filter.render_height ~= 0) then
		filter.uv_mul_val:x(filter.target_width ~= 0 and filter.render_width / filter.target_width or 1.0)
		filter.uv_mul_val:y(filter.target_height ~= 0 and filter.render_height / filter.target_height or 1.0)
		
		filter.uv_offset:x(filter.offset:x() / filter.target_width)
		filter.uv_offset:y(filter.offset:y() / filter.target_height)
	end
	
	gs.vec2_zero(filter.uv_add_val)
	if(filter.offset:x() < 0 and filter.render_width ~= 0) then
		filter.uv_add_val:x(filter.offset:x() / filter.render_width)
	end
	if(filter.offset:y() < 0 and filter.render_height ~= 0) then
		filter.uv_add_val:y(filter.offset:y() / filter.render_height)
	end
end

function filter_update(filter, settings)
	shadow_color = obs.data_get_int(settings, SETTING_SHADOW_COLOR)
	offset_x = obs.data_get_int(settings, SETTING_OFFSET_X)
	offset_y = obs.data_get_int(settings, SETTING_OFFSET_Y)
	filter.hide_image = obs.data_get_bool(settings, SETTING_HIDE_IMAGE)
	filter.hide_shadow = obs.data_get_bool(settings, SETTING_HIDE_SHADOW)
	
	gs.vec2_set(filter.offset, offset_x, offset_y)
	gs.vec4_from_rgba(filter.shadow_color, shadow_color)
	
	calculate_sizes(filter)
end

function filter_create(settings, context)
	filter = { }
	filter.uv_mul_val = vec2.new()
	filter.uv_add_val = vec2.new()
	filter.uv_offset = vec2.new()
	filter.shadow_color = vec4.new()
	filter.offset = vec2.new()
	filter.target_height = 0
	filter.target_width = 0
	filter.context = context
	
	obs.enter_graphics()
	filter.effect = load_effect()
	if (helper.notnull(filter.effect)) then
		filter.shadow_color_param = gs.effect_get_param_by_name(filter.effect, "shadow_color")
		filter.uv_offset_param = gs.effect_get_param_by_name(filter.effect, "uv_offset")
		filter.uv_mul_val_param = gs.effect_get_param_by_name(filter.effect, "uv_mul_val")
		filter.uv_add_val_param = gs.effect_get_param_by_name(filter.effect, "uv_add_val")
		filter.hide_image_param = gs.effect_get_param_by_name(filter.effect, "hide_image")
		filter.hide_shadow_param = gs.effect_get_param_by_name(filter.effect, "hide_shadow")
	end
	obs.leave_graphics()
	
	filter.target_width = 0
	filter.target_height = 0
	
	filter_update(filter, settings)
	return filter
end

function filter_destroy(filter)
	if (helper.notnull(filter.effect)) then
		obs.enter_graphics()
		gs.effect_destroy(filter.effect)
		obs.leave_graphics()
	end
end

function filter_get_width(filter)
	return filter.render_width
end

function filter_get_height(filter)
	return filter.render_height
end

function filter_video_render(filter, effect)
	obs.source_process_filter_begin(filter.context, gs.RGBA, obs.NO_DIRECT_RENDERING)
	
	gs.effect_set_vec4(filter.shadow_color_param, filter.shadow_color)
	gs.effect_set_vec2(filter.uv_offset_param, filter.uv_offset)
	gs.effect_set_vec2(filter.uv_mul_val_param, filter.uv_mul_val)
	gs.effect_set_vec2(filter.uv_add_val_param, filter.uv_add_val)
	gs.effect_set_bool(filter.hide_image_param, filter.hide_image)
	gs.effect_set_bool(filter.hide_shadow_param, filter.hide_shadow)
	
	obs.source_process_filter_end(filter.context, filter.effect, filter.render_width, filter.render_height)
end

function filter_get_properties(filter, props)
	obs.properties_add_color(props, SETTING_SHADOW_COLOR, TEXT_SHADOW_COLOR)
	obs.properties_add_int(props, SETTING_OFFSET_X, TEXT_OFFSET_X, -8192, 8192, 1)
	obs.properties_add_int(props, SETTING_OFFSET_Y, TEXT_OFFSET_Y, -8192, 8192, 1)
	obs.properties_add_bool(props, SETTING_HIDE_IMAGE, TEXT_HIDE_IMAGE)
	obs.properties_add_bool(props, SETTING_HIDE_SHADOW, TEXT_HIDE_SHADOW)
end

function filter_video_tick(filter, seconds)
	target = obs.filter_get_target(filter.context)
	local width, height
	
	if (not helper.notnull(target)) then
		width = 0
		height = 0
	else
		width = obs.source_get_base_width(target)
		height = obs.source_get_base_height(target)
	end
	
	filter.target_width = width
	filter.target_height = height
	
	calculate_sizes(filter)
end

f = { }
f.create = filter_create
f.update = filter_update
f.destroy = filter_destroy
f.get_width = filter_get_width
f.get_height = filter_get_height
f.video_render = filter_video_render
f.get_properties = filter_get_properties
f.video_tick = filter_video_tick
return f
