/*
 *  Copyright (c) 2018-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree.
 */

#include <fizz/protocol/ech/test/TestUtil.h>
#include <fizz/crypto/aead/test/TestUtil.h>

namespace fizz {
namespace ech {
namespace test {

std::vector<Extension> getExtensions(folly::StringPiece hex) {
  auto buf = ::fizz::test::toIOBuf(hex.toString());
  folly::io::Cursor cursor(buf.get());
  Extension ext;
  CHECK_EQ(detail::read(ext, cursor), buf->computeChainDataLength());
  CHECK(cursor.isAtEnd());
  std::vector<Extension> exts;
  exts.push_back(std::move(ext));
  return exts;
}

ECHConfigContentDraft7 getECHConfigContent() {
  HpkeCipherSuite suite{hpke::KDFId::Sha256, hpke::AeadId::TLS_AES_128_GCM_SHA256};
  ECHConfigContentDraft7 echConfigContent;
  echConfigContent.public_name = ::fizz::test::toIOBuf("7075626c69636e616d65");
  echConfigContent.public_key = ::fizz::test::toIOBuf("7075626c69635f6b6579");
  echConfigContent.kem_id = hpke::KEMId::secp256r1;
  echConfigContent.cipher_suites = {suite};
  echConfigContent.maximum_name_length = 1000;
  folly::StringPiece cookie{"002c00080006636f6f6b6965"};
  echConfigContent.extensions = getExtensions(cookie);
  return echConfigContent;
}

ECHConfig getECHConfig() {
  auto configContent = getECHConfigContent();
  ECHConfig testConfig;
  testConfig.version = ECHVersion::V7;
  testConfig.ech_config_content = encode(getECHConfigContent());
  return testConfig;
}

} // namespace test
} // namespace ech
} // namespace fizz