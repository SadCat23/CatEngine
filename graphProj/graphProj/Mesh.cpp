#include "Mesh.h"

Mesh::Mesh(ID3D11Device * device, ID3D11DeviceContext * context, std::vector<Vertex_Texture>& vertices, std::vector<DWORD>& indices)
{
	this->context = context;

	HRESULT hr = this->vertexbuffer.Initialize(device, vertices.data(), vertices.size());
	if (FAILED(hr)) //If error occurred
	{
		ErrorLogger::Log(hr, "Failed to initialize vertex buffer for mesh.");
		return;
	}
	hr = this->indexbuffer.Initialize(device, indices.data(), indices.size());
	if (FAILED(hr)) //If error occurred
	{
		ErrorLogger::Log(hr, "Failed to initialize index buffer for mesh.");
		return;
	}
}

Mesh::Mesh(const Mesh & mesh)
{
	this->context = mesh.context;
	this->indexbuffer = mesh.indexbuffer;
	this->vertexbuffer = mesh.vertexbuffer;
}

void Mesh::Draw()
{
	UINT offset = 0;
	this->context->IASetVertexBuffers(0, 1, this->vertexbuffer.GetAddressOf(), this->vertexbuffer.StridePtr(), &offset);
	this->context->IASetIndexBuffer(this->indexbuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	this->context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->context->DrawIndexed(this->indexbuffer.IndexCount(), 0, 0);
}

void Mesh::RenderDepth()
{

	UINT offset = 0;
	this->context->IASetVertexBuffers(0, 1, this->vertexbuffer.GetAddressOf(), this->vertexbuffer.StridePtr(), &offset);
	this->context->IASetIndexBuffer(this->indexbuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	this->context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->context->DrawIndexed(this->indexbuffer.IndexCount(), 0, 0);

}
