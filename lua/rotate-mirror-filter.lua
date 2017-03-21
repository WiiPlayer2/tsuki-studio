SETTING_ROTATION = "rotation"
SETTING_MIRROR = "mirror"

TEXT_ROTATION = "Rotation"
TEXT_ROTATION_0 = "0 Degrees"
TEXT_ROTATION_90 = "90 Degrees"
TEXT_ROTATION_180 = "180 Degrees"
TEXT_ROTATION_270 = "270 Degrees"
TEXT_MIRROR = "Mirror"

effect_content = [[
uniform float4x4 ViewProj;
uniform texture2d image;

uniform bool mirror;
uniform float2 mat_0;
uniform float2 mat_1;

sampler_state textureSampler {
	Filter    = Linear;
	AddressU  = Wrap;
	AddressV  = Wrap;
};

struct VertData {
	float4 pos : POSITION;
	float2 uv  : TEXCOORD0;
};

VertData VSDefault(VertData v_in)
{
	VertData vert_out;
	vert_out.pos = mul(float4(v_in.pos.xyz, 1.0), ViewProj);
	vert_out.uv  = float2(dot(mat_0, v_in.uv), dot(mat_1, v_in.uv));
    if(mirror)
    {
        vert_out.uv = float2(1.0 - vert_out.uv.x, vert_out.uv.y);
	}
    return vert_out;
}

float4 PSRotate(VertData v_in) : TARGET
{
    return image.Sample(textureSampler, v_in.uv);
}

technique Draw
{
	pass
	{
		vertex_shader = VSDefault(v_in);
		pixel_shader  = PSRotate(v_in);
	}
}
]]

function load_effect()
	local file = os.getenv("TMP") .. os.tmpname()
	local fh = io.open(file, "w")
	fh:write(effect_content)
	fh:flush()
	fh:close()
	local effect = gs.effect_create_from_file(file)
	os.remove(file)
	return effect
end

function calculate_sizes(filter)
	if filter.rotation == 0 or filter.rotation == 180 then
		filter.render_width = filter.target_width
		filter.render_height = filter.target_height
	elseif filter.rotation == 90 or filter.rotation == 270 then
		filter.render_width = filter.target_height
		filter.render_height = filter.target_width
	end
	
	if filter.rotation == 0 then
		gs.vec2_set(filter.mat_0, 1, 0)
		gs.vec2_set(filter.mat_1, 0, 1)
	elseif filter.rotation == 90 then
		gs.vec2_set(filter.mat_0, 0, -1)
		gs.vec2_set(filter.mat_1, 1, 0)
	elseif filter.rotation == 180 then
		gs.vec2_set(filter.mat_0, -1, 0)
		gs.vec2_set(filter.mat_1, 0, -1)
	elseif filter.rotation == 270 then
		gs.vec2_set(filter.mat_0, 0, 1)
		gs.vec2_set(filter.mat_1, -1, 0)
	end
end

function filter_destroy(filter)
	if helper.notnull(filter.effect) then
		obs.enter_graphics()
		gs.effect_destroy(filter.effect)
		obs.leave_graphics()
	end
end

function filter_update(filter, settings)
	filter.rotation = obs.data_get_int(settings, SETTING_ROTATION)
	filter.mirror = obs.data_get_bool(settings, SETTING_MIRROR)
	
	calculate_sizes(filter)
end

function filter_create(settings, context)
	filter = { }
	filter.context = context
	filter.mat_0 = vec2.new()
	filter.mat_1 = vec2.new()
	
	obs.enter_graphics()
	filter.effect = load_effect()
	if helper.notnull(filter.effect) then
		filter.mirror_param = gs.effect_get_param_by_name(filter.effect, "mirror")
		filter.mat_0_param = gs.effect_get_param_by_name(filter.effect, "mat_0")
		filter.mat_1_param = gs.effect_get_param_by_name(filter.effect, "mat_1")
	end
	obs.leave_graphics()
	
	filter.target_width = 0
	filter.target_height = 0
	filter.render_width = 0
	filter.render_height = 0
	
	filter_update(filter, settings)
	return filter
end

function filter_get_width(filter)
	return filter.render_width
end

function filter_get_height(filter)
	return filter.render_height
end

function filter_video_render(filter)
	obs.source_process_filter_begin(filter.context, gs.RGBA, obs.NO_DIRECT_RENDERING)
	
	gs.effect_set_bool(filter.mirror_param, filter.mirror)
	gs.effect_set_vec2(filter.mat_0_param, filter.mat_0)
	gs.effect_set_vec2(filter.mat_1_param, filter.mat_1)
	
	obs.source_process_filter_end(filter.context, filter.effect, filter.render_width, filter.render_height)
end

function filter_get_properties(filter, props)
	list = obs.properties_add_list(props, SETTING_ROTATION, TEXT_ROTATION, obs.COMBO_TYPE_LIST, obs.COMBO_FORMAT_INT)
	obs.property_list_add_int(list, TEXT_ROTATION_0, 0)
	obs.property_list_add_int(list, TEXT_ROTATION_90, 90)
	obs.property_list_add_int(list, TEXT_ROTATION_180, 180)
	obs.property_list_add_int(list, TEXT_ROTATION_270, 270)
	
	obs.properties_add_bool(props, SETTING_MIRROR, TEXT_MIRROR)
end

function filter_video_tick(filter, seconds)
	target = obs.filter_get_target(filter.context)
	
	local width, height
	
	if helper.notnull(target) then
		width = obs.source_get_base_width(target)
		height = obs.source_get_base_height(target)
	else
		width = 0
		height = 0
	end
	
	filter.target_width = width
	filter.target_height = height
	
	calculate_sizes(filter)
end

return {
	create = filter_create,
	destroy = filter_destroy,
	video_render = filter_video_render,
	video_tick = filter_video_tick,
	update = filter_update,
	get_properties = filter_get_properties,
	get_width = filter_get_width,
	get_height = filter_get_height
}