// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: google/protobuf/util/internal/testdata/default_value_test.proto

#include "google/protobuf/util/internal/testdata/default_value_test.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
namespace proto_util_converter {
namespace testing {
class DefaultValueTestDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<DefaultValueTest> _instance;
} _DefaultValueTest_default_instance_;
}  // namespace testing
}  // namespace proto_util_converter
static void InitDefaultsscc_info_DefaultValueTest_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::proto_util_converter::testing::_DefaultValueTest_default_instance_;
    new (ptr) ::proto_util_converter::testing::DefaultValueTest();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::proto_util_converter::testing::DefaultValueTest::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_DefaultValueTest_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, InitDefaultsscc_info_DefaultValueTest_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto}, {}};

static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto[1];
static const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* file_level_enum_descriptors_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::proto_util_converter::testing::DefaultValueTest, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::proto_util_converter::testing::DefaultValueTest, double_value_),
  PROTOBUF_FIELD_OFFSET(::proto_util_converter::testing::DefaultValueTest, repeated_double_),
  PROTOBUF_FIELD_OFFSET(::proto_util_converter::testing::DefaultValueTest, float_value_),
  PROTOBUF_FIELD_OFFSET(::proto_util_converter::testing::DefaultValueTest, int64_value_),
  PROTOBUF_FIELD_OFFSET(::proto_util_converter::testing::DefaultValueTest, uint64_value_),
  PROTOBUF_FIELD_OFFSET(::proto_util_converter::testing::DefaultValueTest, int32_value_),
  PROTOBUF_FIELD_OFFSET(::proto_util_converter::testing::DefaultValueTest, uint32_value_),
  PROTOBUF_FIELD_OFFSET(::proto_util_converter::testing::DefaultValueTest, bool_value_),
  PROTOBUF_FIELD_OFFSET(::proto_util_converter::testing::DefaultValueTest, string_value_),
  PROTOBUF_FIELD_OFFSET(::proto_util_converter::testing::DefaultValueTest, bytes_value_),
  PROTOBUF_FIELD_OFFSET(::proto_util_converter::testing::DefaultValueTest, enum_value_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::proto_util_converter::testing::DefaultValueTest)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::proto_util_converter::testing::_DefaultValueTest_default_instance_),
};

const char descriptor_table_protodef_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto[] =
  "\n\?google/protobuf/util/internal/testdata"
  "/default_value_test.proto\022\034proto_util_co"
  "nverter.testing\"\377\002\n\020DefaultValueTest\022\024\n\014"
  "double_value\030\001 \001(\001\022\027\n\017repeated_double\030\002 "
  "\003(\001\022\023\n\013float_value\030\003 \001(\002\022\023\n\013int64_value\030"
  "\005 \001(\003\022\024\n\014uint64_value\030\007 \001(\004\022\023\n\013int32_val"
  "ue\030\t \001(\005\022\024\n\014uint32_value\030\013 \001(\r\022\022\n\nbool_v"
  "alue\030\r \001(\010\022\024\n\014string_value\030\017 \001(\t\022\027\n\013byte"
  "s_value\030\021 \001(\014B\002\010\001\022N\n\nenum_value\030\022 \001(\0162:."
  "proto_util_converter.testing.DefaultValu"
  "eTest.EnumDefault\">\n\013EnumDefault\022\016\n\nENUM"
  "_FIRST\020\000\022\017\n\013ENUM_SECOND\020\001\022\016\n\nENUM_THIRD\020"
  "\002b\006proto3"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto_deps[1] = {
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto_sccs[1] = {
  &scc_info_DefaultValueTest_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto.base,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto_once;
static bool descriptor_table_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto_initialized = false;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto = {
  &descriptor_table_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto_initialized, descriptor_table_protodef_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto, "google/protobuf/util/internal/testdata/default_value_test.proto", 489,
  &descriptor_table_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto_once, descriptor_table_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto_sccs, descriptor_table_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto_deps, 1, 0,
  schemas, file_default_instances, TableStruct_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto::offsets,
  file_level_metadata_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto, 1, file_level_enum_descriptors_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto, file_level_service_descriptors_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto,
};

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto = (  ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptors(&descriptor_table_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto), true);
namespace proto_util_converter {
namespace testing {
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* DefaultValueTest_EnumDefault_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto);
  return file_level_enum_descriptors_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto[0];
}
bool DefaultValueTest_EnumDefault_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
      return true;
    default:
      return false;
  }
}

#if (__cplusplus < 201703) && (!defined(_MSC_VER) || _MSC_VER >= 1900)
constexpr DefaultValueTest_EnumDefault DefaultValueTest::ENUM_FIRST;
constexpr DefaultValueTest_EnumDefault DefaultValueTest::ENUM_SECOND;
constexpr DefaultValueTest_EnumDefault DefaultValueTest::ENUM_THIRD;
constexpr DefaultValueTest_EnumDefault DefaultValueTest::EnumDefault_MIN;
constexpr DefaultValueTest_EnumDefault DefaultValueTest::EnumDefault_MAX;
constexpr int DefaultValueTest::EnumDefault_ARRAYSIZE;
#endif  // (__cplusplus < 201703) && (!defined(_MSC_VER) || _MSC_VER >= 1900)

// ===================================================================

void DefaultValueTest::InitAsDefaultInstance() {
}
class DefaultValueTest::HasBitSetters {
 public:
};

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int DefaultValueTest::kDoubleValueFieldNumber;
const int DefaultValueTest::kRepeatedDoubleFieldNumber;
const int DefaultValueTest::kFloatValueFieldNumber;
const int DefaultValueTest::kInt64ValueFieldNumber;
const int DefaultValueTest::kUint64ValueFieldNumber;
const int DefaultValueTest::kInt32ValueFieldNumber;
const int DefaultValueTest::kUint32ValueFieldNumber;
const int DefaultValueTest::kBoolValueFieldNumber;
const int DefaultValueTest::kStringValueFieldNumber;
const int DefaultValueTest::kBytesValueFieldNumber;
const int DefaultValueTest::kEnumValueFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

DefaultValueTest::DefaultValueTest()
  : ::PROTOBUF_NAMESPACE_ID::Message(), _internal_metadata_(nullptr) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:proto_util_converter.testing.DefaultValueTest)
}
DefaultValueTest::DefaultValueTest(const DefaultValueTest& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _internal_metadata_(nullptr),
      repeated_double_(from.repeated_double_) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  string_value_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (from.string_value().size() > 0) {
    string_value_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.string_value_);
  }
  bytes_value_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (from.bytes_value().size() > 0) {
    bytes_value_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.bytes_value_);
  }
  ::memcpy(&double_value_, &from.double_value_,
    static_cast<size_t>(reinterpret_cast<char*>(&enum_value_) -
    reinterpret_cast<char*>(&double_value_)) + sizeof(enum_value_));
  // @@protoc_insertion_point(copy_constructor:proto_util_converter.testing.DefaultValueTest)
}

void DefaultValueTest::SharedCtor() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&scc_info_DefaultValueTest_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto.base);
  string_value_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  bytes_value_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  ::memset(&double_value_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&enum_value_) -
      reinterpret_cast<char*>(&double_value_)) + sizeof(enum_value_));
}

DefaultValueTest::~DefaultValueTest() {
  // @@protoc_insertion_point(destructor:proto_util_converter.testing.DefaultValueTest)
  SharedDtor();
}

void DefaultValueTest::SharedDtor() {
  string_value_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  bytes_value_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}

void DefaultValueTest::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const DefaultValueTest& DefaultValueTest::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_DefaultValueTest_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto.base);
  return *internal_default_instance();
}


void DefaultValueTest::Clear() {
// @@protoc_insertion_point(message_clear_start:proto_util_converter.testing.DefaultValueTest)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  repeated_double_.Clear();
  string_value_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  bytes_value_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  ::memset(&double_value_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&enum_value_) -
      reinterpret_cast<char*>(&double_value_)) + sizeof(enum_value_));
  _internal_metadata_.Clear();
}

#if GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
const char* DefaultValueTest::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // double double_value = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 9)) {
          double_value_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<double>(ptr);
          ptr += sizeof(double);
        } else goto handle_unusual;
        continue;
      // repeated double repeated_double = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 18)) {
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::PackedDoubleParser(mutable_repeated_double(), ptr, ctx);
          CHK_(ptr);
        } else if (static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 17) {
          add_repeated_double(::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<double>(ptr));
          ptr += sizeof(double);
        } else goto handle_unusual;
        continue;
      // float float_value = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 29)) {
          float_value_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<float>(ptr);
          ptr += sizeof(float);
        } else goto handle_unusual;
        continue;
      // int64 int64_value = 5;
      case 5:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 40)) {
          int64_value_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // uint64 uint64_value = 7;
      case 7:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 56)) {
          uint64_value_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // int32 int32_value = 9;
      case 9:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 72)) {
          int32_value_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // uint32 uint32_value = 11;
      case 11:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 88)) {
          uint32_value_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // bool bool_value = 13;
      case 13:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 104)) {
          bool_value_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // string string_value = 15;
      case 15:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 122)) {
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParserUTF8(mutable_string_value(), ptr, ctx, "proto_util_converter.testing.DefaultValueTest.string_value");
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // bytes bytes_value = 17 [ctype = CORD];
      case 17:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 138)) {
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(mutable_bytes_value(), ptr, ctx);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // .proto_util_converter.testing.DefaultValueTest.EnumDefault enum_value = 18;
      case 18:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 144)) {
          ::PROTOBUF_NAMESPACE_ID::uint64 val = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
          set_enum_value(static_cast<::proto_util_converter::testing::DefaultValueTest_EnumDefault>(val));
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag, &_internal_metadata_, ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}
#else  // GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
bool DefaultValueTest::MergePartialFromCodedStream(
    ::PROTOBUF_NAMESPACE_ID::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!PROTOBUF_PREDICT_TRUE(EXPRESSION)) goto failure
  ::PROTOBUF_NAMESPACE_ID::uint32 tag;
  // @@protoc_insertion_point(parse_start:proto_util_converter.testing.DefaultValueTest)
  for (;;) {
    ::std::pair<::PROTOBUF_NAMESPACE_ID::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(16383u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // double double_value = 1;
      case 1: {
        if (static_cast< ::PROTOBUF_NAMESPACE_ID::uint8>(tag) == (9 & 0xFF)) {

          DO_((::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::ReadPrimitive<
                   double, ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_DOUBLE>(
                 input, &double_value_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // repeated double repeated_double = 2;
      case 2: {
        if (static_cast< ::PROTOBUF_NAMESPACE_ID::uint8>(tag) == (18 & 0xFF)) {
          DO_((::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::ReadPackedPrimitive<
                   double, ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_DOUBLE>(
                 input, this->mutable_repeated_double())));
        } else if (static_cast< ::PROTOBUF_NAMESPACE_ID::uint8>(tag) == (17 & 0xFF)) {
          DO_((::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::ReadRepeatedPrimitiveNoInline<
                   double, ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_DOUBLE>(
                 1, 18u, input, this->mutable_repeated_double())));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // float float_value = 3;
      case 3: {
        if (static_cast< ::PROTOBUF_NAMESPACE_ID::uint8>(tag) == (29 & 0xFF)) {

          DO_((::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::ReadPrimitive<
                   float, ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_FLOAT>(
                 input, &float_value_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // int64 int64_value = 5;
      case 5: {
        if (static_cast< ::PROTOBUF_NAMESPACE_ID::uint8>(tag) == (40 & 0xFF)) {

          DO_((::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::ReadPrimitive<
                   ::PROTOBUF_NAMESPACE_ID::int64, ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_INT64>(
                 input, &int64_value_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // uint64 uint64_value = 7;
      case 7: {
        if (static_cast< ::PROTOBUF_NAMESPACE_ID::uint8>(tag) == (56 & 0xFF)) {

          DO_((::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::ReadPrimitive<
                   ::PROTOBUF_NAMESPACE_ID::uint64, ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_UINT64>(
                 input, &uint64_value_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // int32 int32_value = 9;
      case 9: {
        if (static_cast< ::PROTOBUF_NAMESPACE_ID::uint8>(tag) == (72 & 0xFF)) {

          DO_((::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::ReadPrimitive<
                   ::PROTOBUF_NAMESPACE_ID::int32, ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_INT32>(
                 input, &int32_value_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // uint32 uint32_value = 11;
      case 11: {
        if (static_cast< ::PROTOBUF_NAMESPACE_ID::uint8>(tag) == (88 & 0xFF)) {

          DO_((::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::ReadPrimitive<
                   ::PROTOBUF_NAMESPACE_ID::uint32, ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_UINT32>(
                 input, &uint32_value_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // bool bool_value = 13;
      case 13: {
        if (static_cast< ::PROTOBUF_NAMESPACE_ID::uint8>(tag) == (104 & 0xFF)) {

          DO_((::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::ReadPrimitive<
                   bool, ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_BOOL>(
                 input, &bool_value_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // string string_value = 15;
      case 15: {
        if (static_cast< ::PROTOBUF_NAMESPACE_ID::uint8>(tag) == (122 & 0xFF)) {
          DO_(::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::ReadString(
                input, this->mutable_string_value()));
          DO_(::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
            this->string_value().data(), static_cast<int>(this->string_value().length()),
            ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::PARSE,
            "proto_util_converter.testing.DefaultValueTest.string_value"));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // bytes bytes_value = 17 [ctype = CORD];
      case 17: {
        if (static_cast< ::PROTOBUF_NAMESPACE_ID::uint8>(tag) == (138 & 0xFF)) {
          DO_(::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::ReadBytes(
                input, this->mutable_bytes_value()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // .proto_util_converter.testing.DefaultValueTest.EnumDefault enum_value = 18;
      case 18: {
        if (static_cast< ::PROTOBUF_NAMESPACE_ID::uint8>(tag) == (144 & 0xFF)) {
          int value = 0;
          DO_((::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::ReadPrimitive<
                   int, ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          set_enum_value(static_cast< ::proto_util_converter::testing::DefaultValueTest_EnumDefault >(value));
        } else {
          goto handle_unusual;
        }
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0) {
          goto success;
        }
        DO_(::PROTOBUF_NAMESPACE_ID::internal::WireFormat::SkipField(
              input, tag, _internal_metadata_.mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:proto_util_converter.testing.DefaultValueTest)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:proto_util_converter.testing.DefaultValueTest)
  return false;
#undef DO_
}
#endif  // GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER

void DefaultValueTest::SerializeWithCachedSizes(
    ::PROTOBUF_NAMESPACE_ID::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:proto_util_converter.testing.DefaultValueTest)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // double double_value = 1;
  if (!(this->double_value() <= 0 && this->double_value() >= 0)) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteDouble(1, this->double_value(), output);
  }

  // repeated double repeated_double = 2;
  if (this->repeated_double_size() > 0) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteTag(2, ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED, output);
    output->WriteVarint32(_repeated_double_cached_byte_size_.load(
        std::memory_order_relaxed));
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteDoubleArray(
      this->repeated_double().data(), this->repeated_double_size(), output);
  }

  // float float_value = 3;
  if (!(this->float_value() <= 0 && this->float_value() >= 0)) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteFloat(3, this->float_value(), output);
  }

  // int64 int64_value = 5;
  if (this->int64_value() != 0) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt64(5, this->int64_value(), output);
  }

  // uint64 uint64_value = 7;
  if (this->uint64_value() != 0) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt64(7, this->uint64_value(), output);
  }

  // int32 int32_value = 9;
  if (this->int32_value() != 0) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32(9, this->int32_value(), output);
  }

  // uint32 uint32_value = 11;
  if (this->uint32_value() != 0) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt32(11, this->uint32_value(), output);
  }

  // bool bool_value = 13;
  if (this->bool_value() != 0) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteBool(13, this->bool_value(), output);
  }

  // string string_value = 15;
  if (this->string_value().size() > 0) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->string_value().data(), static_cast<int>(this->string_value().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "proto_util_converter.testing.DefaultValueTest.string_value");
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteStringMaybeAliased(
      15, this->string_value(), output);
  }

  // bytes bytes_value = 17 [ctype = CORD];
  if (this->bytes_value().size() > 0) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteBytesMaybeAliased(
      17, this->bytes_value(), output);
  }

  // .proto_util_converter.testing.DefaultValueTest.EnumDefault enum_value = 18;
  if (this->enum_value() != 0) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteEnum(
      18, this->enum_value(), output);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::SerializeUnknownFields(
        _internal_metadata_.unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:proto_util_converter.testing.DefaultValueTest)
}

::PROTOBUF_NAMESPACE_ID::uint8* DefaultValueTest::InternalSerializeWithCachedSizesToArray(
    ::PROTOBUF_NAMESPACE_ID::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:proto_util_converter.testing.DefaultValueTest)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // double double_value = 1;
  if (!(this->double_value() <= 0 && this->double_value() >= 0)) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteDoubleToArray(1, this->double_value(), target);
  }

  // repeated double repeated_double = 2;
  if (this->repeated_double_size() > 0) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteTagToArray(
      2,
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED,
      target);
    target = ::PROTOBUF_NAMESPACE_ID::io::CodedOutputStream::WriteVarint32ToArray(
        _repeated_double_cached_byte_size_.load(std::memory_order_relaxed),
         target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      WriteDoubleNoTagToArray(this->repeated_double_, target);
  }

  // float float_value = 3;
  if (!(this->float_value() <= 0 && this->float_value() >= 0)) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteFloatToArray(3, this->float_value(), target);
  }

  // int64 int64_value = 5;
  if (this->int64_value() != 0) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt64ToArray(5, this->int64_value(), target);
  }

  // uint64 uint64_value = 7;
  if (this->uint64_value() != 0) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt64ToArray(7, this->uint64_value(), target);
  }

  // int32 int32_value = 9;
  if (this->int32_value() != 0) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(9, this->int32_value(), target);
  }

  // uint32 uint32_value = 11;
  if (this->uint32_value() != 0) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt32ToArray(11, this->uint32_value(), target);
  }

  // bool bool_value = 13;
  if (this->bool_value() != 0) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteBoolToArray(13, this->bool_value(), target);
  }

  // string string_value = 15;
  if (this->string_value().size() > 0) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->string_value().data(), static_cast<int>(this->string_value().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "proto_util_converter.testing.DefaultValueTest.string_value");
    target =
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteStringToArray(
        15, this->string_value(), target);
  }

  // bytes bytes_value = 17 [ctype = CORD];
  if (this->bytes_value().size() > 0) {
    target =
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteBytesToArray(
        17, this->bytes_value(), target);
  }

  // .proto_util_converter.testing.DefaultValueTest.EnumDefault enum_value = 18;
  if (this->enum_value() != 0) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteEnumToArray(
      18, this->enum_value(), target);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::SerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:proto_util_converter.testing.DefaultValueTest)
  return target;
}

size_t DefaultValueTest::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:proto_util_converter.testing.DefaultValueTest)
  size_t total_size = 0;

  if (_internal_metadata_.have_unknown_fields()) {
    total_size +=
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::ComputeUnknownFieldsSize(
        _internal_metadata_.unknown_fields());
  }
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated double repeated_double = 2;
  {
    unsigned int count = static_cast<unsigned int>(this->repeated_double_size());
    size_t data_size = 8UL * count;
    if (data_size > 0) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
            static_cast<::PROTOBUF_NAMESPACE_ID::int32>(data_size));
    }
    int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(data_size);
    _repeated_double_cached_byte_size_.store(cached_size,
                                    std::memory_order_relaxed);
    total_size += data_size;
  }

  // string string_value = 15;
  if (this->string_value().size() > 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->string_value());
  }

  // bytes bytes_value = 17 [ctype = CORD];
  if (this->bytes_value().size() > 0) {
    total_size += 2 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::BytesSize(
        this->bytes_value());
  }

  // double double_value = 1;
  if (!(this->double_value() <= 0 && this->double_value() >= 0)) {
    total_size += 1 + 8;
  }

  // int64 int64_value = 5;
  if (this->int64_value() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int64Size(
        this->int64_value());
  }

  // float float_value = 3;
  if (!(this->float_value() <= 0 && this->float_value() >= 0)) {
    total_size += 1 + 4;
  }

  // int32 int32_value = 9;
  if (this->int32_value() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->int32_value());
  }

  // uint64 uint64_value = 7;
  if (this->uint64_value() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt64Size(
        this->uint64_value());
  }

  // uint32 uint32_value = 11;
  if (this->uint32_value() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt32Size(
        this->uint32_value());
  }

  // bool bool_value = 13;
  if (this->bool_value() != 0) {
    total_size += 1 + 1;
  }

  // .proto_util_converter.testing.DefaultValueTest.EnumDefault enum_value = 18;
  if (this->enum_value() != 0) {
    total_size += 2 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::EnumSize(this->enum_value());
  }

  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void DefaultValueTest::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:proto_util_converter.testing.DefaultValueTest)
  GOOGLE_DCHECK_NE(&from, this);
  const DefaultValueTest* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<DefaultValueTest>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:proto_util_converter.testing.DefaultValueTest)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:proto_util_converter.testing.DefaultValueTest)
    MergeFrom(*source);
  }
}

void DefaultValueTest::MergeFrom(const DefaultValueTest& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:proto_util_converter.testing.DefaultValueTest)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  repeated_double_.MergeFrom(from.repeated_double_);
  if (from.string_value().size() > 0) {

    string_value_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.string_value_);
  }
  if (from.bytes_value().size() > 0) {

    bytes_value_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.bytes_value_);
  }
  if (!(from.double_value() <= 0 && from.double_value() >= 0)) {
    set_double_value(from.double_value());
  }
  if (from.int64_value() != 0) {
    set_int64_value(from.int64_value());
  }
  if (!(from.float_value() <= 0 && from.float_value() >= 0)) {
    set_float_value(from.float_value());
  }
  if (from.int32_value() != 0) {
    set_int32_value(from.int32_value());
  }
  if (from.uint64_value() != 0) {
    set_uint64_value(from.uint64_value());
  }
  if (from.uint32_value() != 0) {
    set_uint32_value(from.uint32_value());
  }
  if (from.bool_value() != 0) {
    set_bool_value(from.bool_value());
  }
  if (from.enum_value() != 0) {
    set_enum_value(from.enum_value());
  }
}

void DefaultValueTest::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:proto_util_converter.testing.DefaultValueTest)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void DefaultValueTest::CopyFrom(const DefaultValueTest& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:proto_util_converter.testing.DefaultValueTest)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool DefaultValueTest::IsInitialized() const {
  return true;
}

void DefaultValueTest::Swap(DefaultValueTest* other) {
  if (other == this) return;
  InternalSwap(other);
}
void DefaultValueTest::InternalSwap(DefaultValueTest* other) {
  using std::swap;
  _internal_metadata_.Swap(&other->_internal_metadata_);
  repeated_double_.InternalSwap(&other->repeated_double_);
  string_value_.Swap(&other->string_value_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
  bytes_value_.Swap(&other->bytes_value_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
  swap(double_value_, other->double_value_);
  swap(int64_value_, other->int64_value_);
  swap(float_value_, other->float_value_);
  swap(int32_value_, other->int32_value_);
  swap(uint64_value_, other->uint64_value_);
  swap(uint32_value_, other->uint32_value_);
  swap(bool_value_, other->bool_value_);
  swap(enum_value_, other->enum_value_);
}

::PROTOBUF_NAMESPACE_ID::Metadata DefaultValueTest::GetMetadata() const {
  return GetMetadataStatic();
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace testing
}  // namespace proto_util_converter
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::proto_util_converter::testing::DefaultValueTest* Arena::CreateMaybeMessage< ::proto_util_converter::testing::DefaultValueTest >(Arena* arena) {
  return Arena::CreateInternal< ::proto_util_converter::testing::DefaultValueTest >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
