/*
 * Copyright (C) 2017 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#if ENABLE(WEBASSEMBLY)

#include "WasmCodeBlock.h"
#include "WasmMemory.h"
#include <wtf/Expected.h>
#include <wtf/Lock.h>
#include <wtf/SharedTask.h>
#include <wtf/ThreadSafeRefCounted.h>

namespace JSC { namespace Wasm {

struct ModuleInformation;
class Plan;

using SignatureIndex = uint32_t;
    
class Module : public ThreadSafeRefCounted<Module> {
public:
    using ValidationResult = WTF::Expected<RefPtr<Module>, String>;
    typedef void CallbackType(VM&, ValidationResult&&);
    using AsyncValidationCallback = RefPtr<SharedTask<CallbackType>>;

    static ValidationResult validateSync(VM&, Vector<uint8_t>&& source);
    static void validateAsync(VM&, Vector<uint8_t>&& source, Module::AsyncValidationCallback&&);

    static Ref<Module> create(Ref<ModuleInformation>&& moduleInformation)
    {
        return adoptRef(*new Module(WTFMove(moduleInformation)));
    }

    Wasm::SignatureIndex signatureIndexFromFunctionIndexSpace(unsigned functionIndexSpace) const;
    const Wasm::ModuleInformation& moduleInformation() const { return m_moduleInformation.get(); }

    Ref<CodeBlock> compileSync(VM&, MemoryMode);
    void compileAsync(VM&, MemoryMode, CodeBlock::AsyncCompilationCallback&&);

    JS_EXPORT_PRIVATE ~Module();
private:
    Ref<CodeBlock> getOrCreateCodeBlock(VM&, MemoryMode);

    Module(Ref<ModuleInformation>&&);
    Ref<ModuleInformation> m_moduleInformation;
    RefPtr<CodeBlock> m_codeBlocks[Wasm::NumberOfMemoryModes];
    Lock m_lock;
};

} } // namespace JSC::Wasm

#endif // ENABLE(WEBASSEMBLY)
