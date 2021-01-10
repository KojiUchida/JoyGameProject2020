cbuffer cbuff0:register(b0)
{
	matrix mat;
	matrix billboard;
}


//頂点シェーダーからピクセルシェーダ―へのやり取りに使用する構造体
struct VSOutput
{
	float4 svpos:SV_POSITION;
};

struct GSOutput
{
	float4 svpos:SV_POSITION;
	float2 uv:TEXCOORD;
};

VSOutput VSmain(float4 pos:POSITION)
{
	VSOutput output;
	output.svpos = pos;
	return output;
}

static const uint vnum = 4;

static const float4 offset_array[vnum] =
{
	float4(-0.5f,-0.5f,0,0),
	float4(-0.5f,+0.5f,0,0),
	float4(+0.5f,-0.5f,0,0),
	float4(+0.5f,+0.5f,0,0),
};

static const float2 uv_array[vnum] =
{
	float2(0,1),
	float2(0,0),
	float2(1,1),
	float2(1,0),
};

[maxvertexcount(vnum)]   // ジオメトリシェーダーで出力する最大頂点数
void GSmain(point VSOutput input[1],                    // トライアングル リストを構成する頂点配列の入力情報
	inout TriangleStream<GSOutput> output  // 頂点情報を追加するためのストリームオブジェクト
)
{
	GSOutput element;
	for (int i = 0; i < vnum; ++i)
	{
		float4 offset = mul(billboard, offset_array[i]);
		
		element.svpos = input[0].svpos+offset;

		element.svpos = mul(mat, element.svpos);
		element.uv = uv_array[i];
		output.Append(element);
	}
	output.RestartStrip();

}

Texture2D<float4>tex:register(t0);
SamplerState smp:register(s0);

float4 PSmain(GSOutput input) : SV_TARGET
{
	return tex.Sample(smp,input.uv);
	//return float4(1,1,1,1);
}
