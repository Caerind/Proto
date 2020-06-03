#pragma once

#include <vector>
#include <Enlivengine/System/Hash.hpp>
#include <Enlivengine/Math/Noise.hpp>

namespace en
{

enum class ParamType
{
	Bool,
	Int,
	Float
};

struct Block
{
	virtual const char* GetName() const = 0;
	virtual std::vector<ParamType> GetInputs() const = 0;
	virtual std::vector<ParamType> GetOutputs() const = 0;
	virtual void Compute() = 0;

	virtual en::U32 GetTypeID() const { return en::Hash::Meow32(GetName()); }
	virtual en::U32 GetInputCount() const { return static_cast<en::U32>(GetInputs().size()); }
	virtual en::U32 GetOutputCount() const { return static_cast<en::U32>(GetOutputs().size()); }
};

struct ConstantBlock : public Block
{
	static constexpr const char* GetStaticName() { return "Constant"; }
	static en::U32 GetStaticTypeID() { return en::Hash::Meow32(GetStaticName()); }
	static std::vector<ParamType> GetStaticInputs() { return {}; }
	static std::vector<ParamType> GetStaticOutputs() { return { ParamType::Float }; }

	const char* GetName() const override { return GetStaticName(); }
	std::vector<ParamType> GetInputs() const override { return GetStaticInputs(); }
	std::vector<ParamType> GetOutputs() const override { return GetStaticOutputs(); }
	void Compute() override
	{
		output0 = constant;
	}

	en::F32 GetOutput0() const { return output0; }

	ConstantBlock()
		: output0(0.0f)
		, constant(0.0f)
	{
	}

	en::F32 output0;

	en::F32 constant;
};

struct AddBlock : public Block
{
	static constexpr const char* GetStaticName() { return "AddBlock"; }
	static en::U32 GetStaticTypeID() { return en::Hash::Meow32(GetStaticName()); }
	static std::vector<ParamType> GetStaticInputs() { return { ParamType::Float, ParamType::Float }; }
	static std::vector<ParamType> GetStaticOutputs() { return { ParamType::Float }; }

	const char* GetName() const override { return GetStaticName(); }
	std::vector<ParamType> GetInputs() const override { return GetStaticInputs(); }
	std::vector<ParamType> GetOutputs() const override { return GetStaticOutputs(); }
	void Compute() override
	{
		output0 = input0 + input1;
	}

	void SetInput0(en::F32 _input0) { input0 = _input0; }
	void SetInput1(en::F32 _input1) { input1 = _input1; }
	en::F32 GetOutput0() const { return output0; }

	AddBlock()
		: input0(0.0f)
		, input1(0.0f)
		, output0(0.0f)
	{
	}

	en::F32 input0;
	en::F32 input1;
	en::F32 output0;
};








struct Input2DBlock : public Block
{
	static constexpr const char* GetStaticName() { return "Input2D"; }
	static en::U32 GetStaticTypeID() { return en::Hash::Meow32(GetStaticName()); }
	static std::vector<ParamType> GetStaticInputs() { return {}; }
	static std::vector<ParamType> GetStaticOutputs() { return { ParamType::Float, ParamType::Float }; }

	const char* GetName() const override { return GetStaticName(); }
	std::vector<ParamType> GetInputs() const override { return GetStaticInputs(); }
	std::vector<ParamType> GetOutputs() const override { return GetStaticOutputs(); }
	void Compute() override
	{
		output0 = minX;
		output1 = minY;
	}

	en::F32 GetOutput0() const { return output0; }
	en::F32 GetOutput1() const { return output1; }

	Input2DBlock(en::F32 _minX, en::F32 _maxX, en::F32 _epsilonX, en::F32 _minY, en::F32 _maxY, en::F32 _epsilonY)
		: minX(_minX)
		, maxX(_maxX)
		, epsilonX(_epsilonX)
		, minY(_minY)
		, maxY(_maxY)
		, epsilonY(_epsilonY)
	{
	}

	en::F32 output0;
	en::F32 output1;

	en::F32 minX;
	en::F32 maxX;
	en::F32 epsilonX;
	en::F32 minY;
	en::F32 maxY;
	en::F32 epsilonY;
};

struct Noise2DBlock : public Block
{
	static constexpr const char* GetStaticName() { return "Noise2D"; }
	static en::U32 GetStaticTypeID() { return en::Hash::Meow32(GetStaticName()); }
	static std::vector<ParamType> GetStaticInputs() { return { ParamType::Float, ParamType::Float }; }
	static std::vector<ParamType> GetStaticOutputs() { return { ParamType::Float }; }

	const char* GetName() const override { return GetStaticName(); }
	std::vector<ParamType> GetInputs() const override { return GetStaticInputs(); }
	std::vector<ParamType> GetOutputs() const override { return GetStaticOutputs(); }
	void Compute() override 
	{ 
		output0 = noise.Get(input0, input1);
	}

	void SetInput0(en::F32 _input0) { input0 = _input0; }
	void SetInput1(en::F32 _input1) { input1 = _input1; }
	en::F32 GetOutput0() const { return output0; }


	Noise2DBlock(en::Noise& _noise)
		: noise(_noise)
	{
	}

	en::F32 input0;
	en::F32 input1;
	en::F32 output0;

	en::Noise& noise;
};

} // namespace en