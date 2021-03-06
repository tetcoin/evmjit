#include <evmjit/JIT-c.h>
#include <cassert>
#include <evmjit/JIT.h>

extern "C"
{
using namespace dev::evmjit;

evmjit_runtime_data* evmjit_create_runtime_data()
{
	auto data = new(std::nothrow) RuntimeData();
	return reinterpret_cast<evmjit_runtime_data*>(data);
}	

void evmjit_destroy_runtime_data(evmjit_runtime_data* _data)
{
	auto data = reinterpret_cast<RuntimeData*>(_data);
	delete data;
}

evmjit_context* evmjit_create_context(evmjit_runtime_data* _data, void* _env)
{
	auto data = reinterpret_cast<RuntimeData*>(_data);
	auto env  = reinterpret_cast<Env*>(_env);

	assert(data && "Pointer to runtime data must not be null");
	if (!data)
		return nullptr;

	// TODO: Make sure ExecutionEngine constructor does not throw + make JIT/ExecutionEngine interface all nothrow
	auto context = new(std::nothrow) ExecutionContext{*data, env};
	return reinterpret_cast<evmjit_context*>(context);
}

void evmjit_destroy_context(evmjit_context* _context)
{
	auto context = reinterpret_cast<ExecutionContext*>(_context);
	delete context;
}

evmjit_return_code evmjit_exec(evmjit_context* _context)
{
	auto context = reinterpret_cast<ExecutionContext*>(_context);

	assert(context && "Invalid context");
	if (!context)
		return UnexpectedException;

	try
	{
		auto returnCode = JIT::exec(*context);
		return static_cast<evmjit_return_code>(returnCode);
	}
	catch(...)
	{
		return UnexpectedException;
	}
}

}
