struct VS_IN
{
	float4 pos : POSITION0;
	
};

struct PS_IN
{
	float4 pos : SV_POSITION;
	 float4 col : COLOR;
};

PS_IN VSMain( VS_IN input )
{
	PS_IN output = (PS_IN)0;
	
	output.pos = input.pos;

	return output;
}

float4 PSMain( PS_IN input ) : SV_Target
{
	float4 col = input.col;
	return float4 ( 1.0f, 1.0f, 1.0f, 1.0f );
}
