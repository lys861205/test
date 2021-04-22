local tcp_table = DissectorTable.get("tcp.port")
local xgram_proto = Proto("xgram", "xgram protocol", "xgram protocol")

local xgram_port = 9527
local idField = ProtoField.uint16("id", "id", base.DEC)
local versionField = ProtoField.uint16("version", "version", base.DEC)
local logidField = ProtoField.uint32("logid", "logid", base.DEC)
local providerField = ProtoField.string("provider", "provider")
local magicnumField = ProtoField.uint32("magicnum", "magicnum", base.DEC)
local reservedField = ProtoField.uint32("reserved", "reserved", base.DEC)
local bodylenField = ProtoField.uint32("bodylen", "bodylen", base.DEC)
--local bodyField = ProtoField.string("body", "body")


local data_dis = Dissector.get("data")

local function is_request(pinfo)
	if pinfo.dst_port == 9527 then 
		return true
	else 
		return false
	end
end
--xgram_proto.fields = {idField, versionField, logidField, providerField, magicnumField, reservedField, bodylenField, bodyField}
xgram_proto.fields = {idField, versionField, logidField, providerField, magicnumField, reservedField, bodylenField}
local function Xgram_dissector(buffer, pinfo, tree)
	
	local len = buffer:len()
	if len < 36 then return false end 
	local magic_num_buf = buffer:range(24, 4)
	if magic_num_buf:le_uint() ~=  0xffee7799 then return false end 
	
	--pinfo.cols.protocol:set("xgram")
	if is_request(pinfo) then 
		--pinfo.cols.info:append("[REQUEST]")
		pinfo.cols.protocol:set("xgram request")
	else 
		--pinfo.cols.info:append("[RESPONSE]")
		pinfo.cols.protocol:set("xgram response")
	end 
	local proto_tree = tree:add(xgram_proto, buffer(0, 36), "xgram protocol")
	local offset = 0
	proto_tree:add_le(idField, buffer(offset, 2))
	offset = offset + 2
	proto_tree:add_le(versionField, buffer(offset, 2))
	offset = offset + 2
	proto_tree:add_le(logidField, buffer(offset, 4))
	offset = offset + 4
	proto_tree:add_le(providerField, buffer(offset, 16))
	offset = offset + 16
	proto_tree:add_le(magicnumField, buffer(offset, 4))
	offset = offset + 4
	proto_tree:add_le(reservedField, buffer(offset, 4))
	offset = offset + 4
	proto_tree:add_le(bodylenField, buffer(offset, 4))
	offset = offset + 4
	data_dis:call(buffer(offset):tvb(), pinfo, tree)
	return true
end
-- 协议分析器

function xgram_proto.dissector(buffer, pinfo, tree)
	if not Xgram_dissector(buffer, pinfo, tree) then 
		data_dis:call(buffer, pinfo, tree)
	end 
end

-- 增加协议到wiresharke
tcp_table:add(xgram_port, xgram_proto)