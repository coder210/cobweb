// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: google/protobuf/unittest_preserve_unknown_enum2.proto

#include "google/protobuf/unittest_preserve_unknown_enum2.pb.h"

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
namespace proto2_preserve_unknown_enum_unittest {
class MyMessageDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<MyMessage> _instance;
  int oneof_e_1_;
  int oneof_e_2_;
} _MyMessage_default_instance_;
}  // namespace proto2_preserve_unknown_enum_unittest
static void InitDefaultsscc_info_MyMessage_google_2fprotobuf_2funittest_5fpreserve_5funknown_5fenum2_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::proto2_preserve_unknown_enum_unittest::_MyMessage_default_instance_;
    new (ptr) ::proto2_preserve_unknown_enum_unittest::MyMessage();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::proto2_preserve_unknown_enum_unittest::MyMessage::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_MyMessage_google_2fprotobuf_2funittest_5fpreserve_5funknown_5fenum2_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, InitDefaultsscc_info_MyMessage_google_2fprotobuf_2funittest_5fpreserve_5funknown_5fenum2_2eproto}, {}};

static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_google_2fprotobuf_2funittest_5fpreserve_5funknown_5fenum2_2eproto[1];
static const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* file_level_enum_descriptors_google_2fprotobuf_2funittest_5fpreserve_5funknown_5fenum2_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_google_2fprotobuf_2funittest_5fpreserve_5funknown_5fenum2_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_google_2fprotobuf_2funittest_5fpreserve_5funknown_5fenum2_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  PROTOBUF_FIELD_OFFSET(::proto2_preserve_unknown_enum_unittest::MyMessage, _has_bits_),
  PROTOBUF_FIELD_OFFSET(::proto2_preserve_unknown_enum_unittest::MyMessage, _internal_metadata_),
  ~0u,  // no _extensions_
  PROTOBUF_FIELD_OFFSET(::proto2_preserve_unknown_enum_unittest::MyMessage, _oneof_case_[0]),
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::proto2_preserve_unknown_enum_unittest::MyMessage, e_),
  PROTOBUF_FIELD_OFFSET(::proto2_preserve_unknown_enum_unittest::MyMessage, repeated_e_),
  PROTOBUF_FIELD_OFFSET(::proto2_preserve_unknown_enum_unittest::MyMessage, repeated_packed_e_),
  PROTOBUF_FIELD_OFFSET(::proto2_preserve_unknown_enum_unittest::MyMessage, repeated_packed_unexpected_e_),
  offsetof(::proto2_preserve_unknown_enum_unittest::MyMessageDefaultTypeInternal, oneof_e_1_),
  offsetof(::proto2_preserve_unknown_enum_unittest::MyMessageDefaultTypeInternal, oneof_e_2_),
  PROTOBUF_FIELD_OFFSET(::proto2_preserve_unknown_enum_unittest::MyMessage, o_),
  0,
  ~0u,
  ~0u,
  ~0u,
  ~0u,
  ~0u,
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, 12, sizeof(::proto2_preserve_unknown_enum_unittest::MyMessage)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::proto2_preserve_unknown_enum_unittest::_MyMessage_default_instance_),
};

const char descriptor_table_protodef_google_2fprotobuf_2funittest_5fpreserve_5funknown_5fenum2_2eproto[] =
  "\n5google/protobuf/unittest_preserve_unkn"
  "own_enum2.proto\022%proto2_preserve_unknown"
  "_enum_unittest\"\270\003\n\tMyMessage\0228\n\001e\030\001 \001(\0162"
  "-.proto2_preserve_unknown_enum_unittest."
  "MyEnum\022A\n\nrepeated_e\030\002 \003(\0162-.proto2_pres"
  "erve_unknown_enum_unittest.MyEnum\022L\n\021rep"
  "eated_packed_e\030\003 \003(\0162-.proto2_preserve_u"
  "nknown_enum_unittest.MyEnumB\002\020\001\022S\n\034repea"
  "ted_packed_unexpected_e\030\004 \003(\0162-.proto2_p"
  "reserve_unknown_enum_unittest.MyEnum\022B\n\t"
  "oneof_e_1\030\005 \001(\0162-.proto2_preserve_unknow"
  "n_enum_unittest.MyEnumH\000\022B\n\toneof_e_2\030\006 "
  "\001(\0162-.proto2_preserve_unknown_enum_unitt"
  "est.MyEnumH\000B\003\n\001o*#\n\006MyEnum\022\007\n\003FOO\020\000\022\007\n\003"
  "BAR\020\001\022\007\n\003BAZ\020\002"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_google_2fprotobuf_2funittest_5fpreserve_5funknown_5fenum2_2eproto_deps[1] = {
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_google_2fprotobuf_2funittest_5fpreserve_5funknown_5fenum2_2eproto_sccs[1] = {
  &scc_info_MyMessage_google_2fprotobuf_2funittest_5fpreserve_5funknown_5fenum2_2eproto.base,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_google_2fprotobuf_2funittest_5fpreserve_5funknown_5fenum2_2eproto_once;
static bool descriptor_table_google_2fprotobuf_2funittest_5fpreserve_5funknown_5fenum2_2eproto_initialized = false;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_google_2fprotobuf_2funittest_5fpreserve_5funknown_5fenum2_2eproto = {
  &descriptor_table_google_2fprotobuf_2funittest_5fpreserve_5funknown_5fenum2_2eproto_initialized, descriptor_table_protodef_google_2fprotobuf_2funittest_5fpreserve_5funknown_5fenum2_2eproto, "google/protobuf/unittest_preserve_unknown_enum2.proto", 574,
  &descriptor_table_google_2fprotobuf_2funittest_5fpreserve_5funknown_5fenum2_2eproto_once, descriptor_table_google_2fprotobuf_2funittest_5fpreserve_5funknown_5fenum2_2eproto_sccs, descriptor_table_google_2fprotobuf_2funittest_5fpreserve_5funknown_5fenum2_2eproto_deps, 1, 0,
  schemas, file_default_instances, TableStruct_google_2fprotobuf_2funittest_5fpreserve_5funknown_5fenum2_2eproto::offsets,
  file_level_metadata_google_2fprotobuf_2funittest_5fpreserve_5funknown_5fenum2_2eproto, 1, file_level_enum_descriptors_google_2fprotobuf_2funittest_5fpreserve_5funknown_5fenum2_2eproto, file_level_service_descriptors_google_2fprotobuf_2funittest_5fpreserve_5funknown_5fenum2_2eproto,
};

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_google_2fprotobuf_2funittest_5fpreserve_5funknown_5fenum2_2eproto = (  ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptors(&descriptor_table_google_2fprotobuf_2funittest_5fpreserve_5funknown_5fenum2_2eproto), true);
namespace proto2_preserve_unknown_enum_unittest {
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* MyEnum_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_google_2fprotobuf_2funittest_5fpreserve_5funknown_5fenum2_2eproto);
  return file_level_enum_descriptors_google_2fprotobuf_2funittest_5fpreserve_5funknown_5fenum2_2eproto[0];
}
bool MyEnum_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
      return true;
    default:
      return false;
  }
}


// ===================================================================

void MyMessage::InitAsDefaultInstance() {
  ::proto2_preserve_unknown_enum_unittest::_MyMessage_default_instance_.oneof_e_1_ = 0;
  ::proto2_preserve_unknown_enum_unittest::_MyMessage_default_instance_.oneof_e_2_ = 0;
}
class MyMessage::HasBitSetters {
 public:
  using HasBits = decltype(std::declval<MyMessage>()._has_bits_);
  static void set_has_e(HasBits* has_bits) {
    (*has_bits)[0] |= 1u;
  }
};

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int MyMessage::kEFieldNumber;
const int MyMessage::kRepeatedEFieldNumber;
const int MyMessage::kRepeatedPackedEFieldNumber;
const int MyMessage::kRepeatedPackedUnexpectedEFieldNumber;
const int MyMessage::kOneofE1FieldNumber;
const int MyMessage::kOneofE2FieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

MyMessage::MyMessage()
  : ::PROTOBUF_NAMESPACE_ID::Message(), _internal_metadata_(nullptr) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:proto2_preserve_unknown_enum_unittest.MyMessage)
}
MyMessage::MyMessage(const MyMessage& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _internal_metadata_(nullptr),
      _has_bits_(from._has_bits_),
      repeated_e_(from.repeated_e_),
      repeated_packed_e_(from.repeated_packed_e_),
      repeated_packed_unexpected_e_(from.repeated_packed_unexpected_e_) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  e_ = from.e_;
  clear_has_o();
  switch (from.o_case()) {
    case kOneofE1: {
      set_oneof_e_1(from.oneof_e_1());
      break;
    }
    case kOneofE2: {
      set_oneof_e_2(from.oneof_e_2());
      break;
    }
    case O_NOT_SET: {
      break;
    }
  }
  // @@protoc_insertion_point(copy_constructor:proto2_preserve_unknown_enum_unittest.MyMessage)
}

void MyMessage::SharedCtor() {
  e_ = 0;
  clear_has_o();
}

MyMessage::~MyMessage() {
  // @@protoc_insertion_point(destructor:proto2_preserve_unknown_enum_unittest.MyMessage)
  SharedDtor();
}

void MyMessage::SharedDtor() {
  if (has_o()) {
    clear_o();
  }
}

void MyMessage::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const MyMessage& MyMessage::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_MyMessage_google_2fprotobuf_2funittest_5fpreserve_5funknown_5fenum2_2eproto.base);
  return *internal_default_instance();
}


void MyMessage::clear_o() {
// @@protoc_insertion_point(one_of_clear_start:proto2_preserve_unknown_enum_unittest.MyMessage)
  switch (o_case()) {
    case kOneofE1: {
      // No need to clear
      break;
    }
    case kOneofE2: {
      // No need to clear
      break;
    }
    case O_NOT_SET: {
      break;
    }
  }
  _oneof_case_[0] = O_NOT_SET;
}


void MyMessage::Clear() {
// @@protoc_insertion_point(message_clear_start:proto2_preserve_unknown_enum_unittest.MyMessage)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  repeated_e_.Clear();
  repeated_packed_e_.Clear();
  repeated_packed_unexpected_e_.Clear();
  e_ = 0;
  clear_o();
  _has_bits_.Clear();
  _internal_metadata_.Clear();
}

#if GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
const char* MyMessage::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  HasBitSetters::HasBits has_bits{};
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // optional .proto2_preserve_unknown_enum_unittest.MyEnum e = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          ::PROTOBUF_NAMESPACE_ID::uint64 val = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
          if (PROTOBUF_PREDICT_TRUE(::proto2_preserve_unknown_enum_unittest::MyEnum_IsValid(val))) {
            set_e(static_cast<::proto2_preserve_unknown_enum_unittest::MyEnum>(val));
          } else {
            ::PROTOBUF_NAMESPACE_ID::internal::WriteVarint(1, val, mutable_unknown_fields());
          }
        } else goto handle_unusual;
        continue;
      // repeated .proto2_preserve_unknown_enum_unittest.MyEnum repeated_e = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 16)) {
          ptr -= 1;
          do {
            ptr += 1;
            ::PROTOBUF_NAMESPACE_ID::uint64 val = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
            CHK_(ptr);
            if (PROTOBUF_PREDICT_TRUE(::proto2_preserve_unknown_enum_unittest::MyEnum_IsValid(val))) {
              add_repeated_e(static_cast<::proto2_preserve_unknown_enum_unittest::MyEnum>(val));
            } else {
              ::PROTOBUF_NAMESPACE_ID::internal::WriteVarint(2, val, mutable_unknown_fields());
            }
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<::PROTOBUF_NAMESPACE_ID::uint8>(ptr) == 16);
        } else if (static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 18) {
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::PackedEnumParser(mutable_repeated_e(), ptr, ctx, ::proto2_preserve_unknown_enum_unittest::MyEnum_IsValid, mutable_unknown_fields(), 2);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // repeated .proto2_preserve_unknown_enum_unittest.MyEnum repeated_packed_e = 3 [packed = true];
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 26)) {
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::PackedEnumParser(mutable_repeated_packed_e(), ptr, ctx, ::proto2_preserve_unknown_enum_unittest::MyEnum_IsValid, mutable_unknown_fields(), 3);
          CHK_(ptr);
        } else if (static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 24) {
          ::PROTOBUF_NAMESPACE_ID::uint64 val = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
          if (PROTOBUF_PREDICT_TRUE(::proto2_preserve_unknown_enum_unittest::MyEnum_IsValid(val))) {
            add_repeated_packed_e(static_cast<::proto2_preserve_unknown_enum_unittest::MyEnum>(val));
          } else {
            ::PROTOBUF_NAMESPACE_ID::internal::WriteVarint(3, val, mutable_unknown_fields());
          }
        } else goto handle_unusual;
        continue;
      // repeated .proto2_preserve_unknown_enum_unittest.MyEnum repeated_packed_unexpected_e = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 32)) {
          ptr -= 1;
          do {
            ptr += 1;
            ::PROTOBUF_NAMESPACE_ID::uint64 val = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
            CHK_(ptr);
            if (PROTOBUF_PREDICT_TRUE(::proto2_preserve_unknown_enum_unittest::MyEnum_IsValid(val))) {
              add_repeated_packed_unexpected_e(static_cast<::proto2_preserve_unknown_enum_unittest::MyEnum>(val));
            } else {
              ::PROTOBUF_NAMESPACE_ID::internal::WriteVarint(4, val, mutable_unknown_fields());
            }
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<::PROTOBUF_NAMESPACE_ID::uint8>(ptr) == 32);
        } else if (static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 34) {
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::PackedEnumParser(mutable_repeated_packed_unexpected_e(), ptr, ctx, ::proto2_preserve_unknown_enum_unittest::MyEnum_IsValid, mutable_unknown_fields(), 4);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // optional .proto2_preserve_unknown_enum_unittest.MyEnum oneof_e_1 = 5;
      case 5:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 40)) {
          ::PROTOBUF_NAMESPACE_ID::uint64 val = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
          if (PROTOBUF_PREDICT_TRUE(::proto2_preserve_unknown_enum_unittest::MyEnum_IsValid(val))) {
            set_oneof_e_1(static_cast<::proto2_preserve_unknown_enum_unittest::MyEnum>(val));
          } else {
            ::PROTOBUF_NAMESPACE_ID::internal::WriteVarint(5, val, mutable_unknown_fields());
          }
        } else goto handle_unusual;
        continue;
      // optional .proto2_preserve_unknown_enum_unittest.MyEnum oneof_e_2 = 6;
      case 6:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 48)) {
          ::PROTOBUF_NAMESPACE_ID::uint64 val = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
          if (PROTOBUF_PREDICT_TRUE(::proto2_preserve_unknown_enum_unittest::MyEnum_IsValid(val))) {
            set_oneof_e_2(static_cast<::proto2_preserve_unknown_enum_unittest::MyEnum>(val));
          } else {
            ::PROTOBUF_NAMESPACE_ID::internal::WriteVarint(6, val, mutable_unknown_fields());
          }
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
  _has_bits_.Or(has_bits);
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}
#else  // GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
bool MyMessage::MergePartialFromCodedStream(
    ::PROTOBUF_NAMESPACE_ID::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!PROTOBUF_PREDICT_TRUE(EXPRESSION)) goto failure
  ::PROTOBUF_NAMESPACE_ID::uint32 tag;
  // @@protoc_insertion_point(parse_start:proto2_preserve_unknown_enum_unittest.MyMessage)
  for (;;) {
    ::std::pair<::PROTOBUF_NAMESPACE_ID::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional .proto2_preserve_unknown_enum_unittest.MyEnum e = 1;
      case 1: {
        if (static_cast< ::PROTOBUF_NAMESPACE_ID::uint8>(tag) == (8 & 0xFF)) {
          int value = 0;
          DO_((::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::ReadPrimitive<
                   int, ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::proto2_preserve_unknown_enum_unittest::MyEnum_IsValid(value)) {
            set_e(static_cast< ::proto2_preserve_unknown_enum_unittest::MyEnum >(value));
          } else {
            mutable_unknown_fields()->AddVarint(
                1, static_cast<::PROTOBUF_NAMESPACE_ID::uint64>(value));
          }
        } else {
          goto handle_unusual;
        }
        break;
      }

      // repeated .proto2_preserve_unknown_enum_unittest.MyEnum repeated_e = 2;
      case 2: {
        if (static_cast< ::PROTOBUF_NAMESPACE_ID::uint8>(tag) == (16 & 0xFF)) {
          int value = 0;
          DO_((::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::ReadPrimitive<
                   int, ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::proto2_preserve_unknown_enum_unittest::MyEnum_IsValid(value)) {
            add_repeated_e(static_cast< ::proto2_preserve_unknown_enum_unittest::MyEnum >(value));
          } else {
            mutable_unknown_fields()->AddVarint(
                2, static_cast<::PROTOBUF_NAMESPACE_ID::uint64>(value));
          }
        } else if (static_cast< ::PROTOBUF_NAMESPACE_ID::uint8>(tag) == (18 & 0xFF)) {
          DO_((::PROTOBUF_NAMESPACE_ID::internal::WireFormat::ReadPackedEnumPreserveUnknowns(
                 input,
                 2,
                 ::proto2_preserve_unknown_enum_unittest::MyEnum_IsValid,
                 mutable_unknown_fields(),
                 this->mutable_repeated_e())));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // repeated .proto2_preserve_unknown_enum_unittest.MyEnum repeated_packed_e = 3 [packed = true];
      case 3: {
        if (static_cast< ::PROTOBUF_NAMESPACE_ID::uint8>(tag) == (26 & 0xFF)) {
          ::PROTOBUF_NAMESPACE_ID::uint32 length;
          DO_(input->ReadVarint32(&length));
          ::PROTOBUF_NAMESPACE_ID::io::CodedInputStream::Limit limit = input->PushLimit(static_cast<int>(length));
          while (input->BytesUntilLimit() > 0) {
            int value = 0;
            DO_((::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::ReadPrimitive<
                   int, ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
            if (::proto2_preserve_unknown_enum_unittest::MyEnum_IsValid(value)) {
              add_repeated_packed_e(static_cast< ::proto2_preserve_unknown_enum_unittest::MyEnum >(value));
            } else {
            mutable_unknown_fields()->AddVarint(
                3, static_cast<::PROTOBUF_NAMESPACE_ID::uint64>(value));
            }
          }
          input->PopLimit(limit);
        } else if (static_cast< ::PROTOBUF_NAMESPACE_ID::uint8>(tag) == (24 & 0xFF)) {
          int value = 0;
          DO_((::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::ReadPrimitive<
                   int, ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::proto2_preserve_unknown_enum_unittest::MyEnum_IsValid(value)) {
            add_repeated_packed_e(static_cast< ::proto2_preserve_unknown_enum_unittest::MyEnum >(value));
          } else {
            mutable_unknown_fields()->AddVarint(
                3, static_cast<::PROTOBUF_NAMESPACE_ID::uint64>(value));
          }
        } else {
          goto handle_unusual;
        }
        break;
      }

      // repeated .proto2_preserve_unknown_enum_unittest.MyEnum repeated_packed_unexpected_e = 4;
      case 4: {
        if (static_cast< ::PROTOBUF_NAMESPACE_ID::uint8>(tag) == (32 & 0xFF)) {
          int value = 0;
          DO_((::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::ReadPrimitive<
                   int, ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::proto2_preserve_unknown_enum_unittest::MyEnum_IsValid(value)) {
            add_repeated_packed_unexpected_e(static_cast< ::proto2_preserve_unknown_enum_unittest::MyEnum >(value));
          } else {
            mutable_unknown_fields()->AddVarint(
                4, static_cast<::PROTOBUF_NAMESPACE_ID::uint64>(value));
          }
        } else if (static_cast< ::PROTOBUF_NAMESPACE_ID::uint8>(tag) == (34 & 0xFF)) {
          DO_((::PROTOBUF_NAMESPACE_ID::internal::WireFormat::ReadPackedEnumPreserveUnknowns(
                 input,
                 4,
                 ::proto2_preserve_unknown_enum_unittest::MyEnum_IsValid,
                 mutable_unknown_fields(),
                 this->mutable_repeated_packed_unexpected_e())));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional .proto2_preserve_unknown_enum_unittest.MyEnum oneof_e_1 = 5;
      case 5: {
        if (static_cast< ::PROTOBUF_NAMESPACE_ID::uint8>(tag) == (40 & 0xFF)) {
          int value = 0;
          DO_((::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::ReadPrimitive<
                   int, ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::proto2_preserve_unknown_enum_unittest::MyEnum_IsValid(value)) {
            set_oneof_e_1(static_cast< ::proto2_preserve_unknown_enum_unittest::MyEnum >(value));
          } else {
            mutable_unknown_fields()->AddVarint(
                5, static_cast<::PROTOBUF_NAMESPACE_ID::uint64>(value));
          }
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional .proto2_preserve_unknown_enum_unittest.MyEnum oneof_e_2 = 6;
      case 6: {
        if (static_cast< ::PROTOBUF_NAMESPACE_ID::uint8>(tag) == (48 & 0xFF)) {
          int value = 0;
          DO_((::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::ReadPrimitive<
                   int, ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::proto2_preserve_unknown_enum_unittest::MyEnum_IsValid(value)) {
            set_oneof_e_2(static_cast< ::proto2_preserve_unknown_enum_unittest::MyEnum >(value));
          } else {
            mutable_unknown_fields()->AddVarint(
                6, static_cast<::PROTOBUF_NAMESPACE_ID::uint64>(value));
          }
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
  // @@protoc_insertion_point(parse_success:proto2_preserve_unknown_enum_unittest.MyMessage)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:proto2_preserve_unknown_enum_unittest.MyMessage)
  return false;
#undef DO_
}
#endif  // GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER

void MyMessage::SerializeWithCachedSizes(
    ::PROTOBUF_NAMESPACE_ID::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:proto2_preserve_unknown_enum_unittest.MyMessage)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // optional .proto2_preserve_unknown_enum_unittest.MyEnum e = 1;
  if (cached_has_bits & 0x00000001u) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteEnum(
      1, this->e(), output);
  }

  // repeated .proto2_preserve_unknown_enum_unittest.MyEnum repeated_e = 2;
  for (int i = 0, n = this->repeated_e_size(); i < n; i++) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteEnum(
      2, this->repeated_e(i), output);
  }

  // repeated .proto2_preserve_unknown_enum_unittest.MyEnum repeated_packed_e = 3 [packed = true];
  if (this->repeated_packed_e_size() > 0) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteTag(
      3,
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED,
      output);
    output->WriteVarint32(_repeated_packed_e_cached_byte_size_.load(
        std::memory_order_relaxed));
  }
  for (int i = 0, n = this->repeated_packed_e_size(); i < n; i++) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteEnumNoTag(
      this->repeated_packed_e(i), output);
  }

  // repeated .proto2_preserve_unknown_enum_unittest.MyEnum repeated_packed_unexpected_e = 4;
  for (int i = 0, n = this->repeated_packed_unexpected_e_size(); i < n; i++) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteEnum(
      4, this->repeated_packed_unexpected_e(i), output);
  }

  switch (o_case()) {
    case kOneofE1:
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteEnum(
        5, this->oneof_e_1(), output);
      break;
    case kOneofE2:
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteEnum(
        6, this->oneof_e_2(), output);
      break;
    default: ;
  }
  if (_internal_metadata_.have_unknown_fields()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::SerializeUnknownFields(
        _internal_metadata_.unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:proto2_preserve_unknown_enum_unittest.MyMessage)
}

::PROTOBUF_NAMESPACE_ID::uint8* MyMessage::InternalSerializeWithCachedSizesToArray(
    ::PROTOBUF_NAMESPACE_ID::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:proto2_preserve_unknown_enum_unittest.MyMessage)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // optional .proto2_preserve_unknown_enum_unittest.MyEnum e = 1;
  if (cached_has_bits & 0x00000001u) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteEnumToArray(
      1, this->e(), target);
  }

  // repeated .proto2_preserve_unknown_enum_unittest.MyEnum repeated_e = 2;
  target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteEnumToArray(
    2, this->repeated_e_, target);

  // repeated .proto2_preserve_unknown_enum_unittest.MyEnum repeated_packed_e = 3 [packed = true];
  if (this->repeated_packed_e_size() > 0) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteTagToArray(
      3,
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED,
      target);
    target = ::PROTOBUF_NAMESPACE_ID::io::CodedOutputStream::WriteVarint32ToArray(      _repeated_packed_e_cached_byte_size_.load(std::memory_order_relaxed),
        target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteEnumNoTagToArray(
      this->repeated_packed_e_, target);
  }

  // repeated .proto2_preserve_unknown_enum_unittest.MyEnum repeated_packed_unexpected_e = 4;
  target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteEnumToArray(
    4, this->repeated_packed_unexpected_e_, target);

  switch (o_case()) {
    case kOneofE1:
      target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteEnumToArray(
        5, this->oneof_e_1(), target);
      break;
    case kOneofE2:
      target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteEnumToArray(
        6, this->oneof_e_2(), target);
      break;
    default: ;
  }
  if (_internal_metadata_.have_unknown_fields()) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::SerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:proto2_preserve_unknown_enum_unittest.MyMessage)
  return target;
}

size_t MyMessage::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:proto2_preserve_unknown_enum_unittest.MyMessage)
  size_t total_size = 0;

  if (_internal_metadata_.have_unknown_fields()) {
    total_size +=
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::ComputeUnknownFieldsSize(
        _internal_metadata_.unknown_fields());
  }
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated .proto2_preserve_unknown_enum_unittest.MyEnum repeated_e = 2;
  {
    size_t data_size = 0;
    unsigned int count = static_cast<unsigned int>(this->repeated_e_size());for (unsigned int i = 0; i < count; i++) {
      data_size += ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::EnumSize(
        this->repeated_e(static_cast<int>(i)));
    }
    total_size += (1UL * count) + data_size;
  }

  // repeated .proto2_preserve_unknown_enum_unittest.MyEnum repeated_packed_e = 3 [packed = true];
  {
    size_t data_size = 0;
    unsigned int count = static_cast<unsigned int>(this->repeated_packed_e_size());for (unsigned int i = 0; i < count; i++) {
      data_size += ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::EnumSize(
        this->repeated_packed_e(static_cast<int>(i)));
    }
    if (data_size > 0) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
            static_cast<::PROTOBUF_NAMESPACE_ID::int32>(data_size));
    }
    int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(data_size);
    _repeated_packed_e_cached_byte_size_.store(cached_size,
                                    std::memory_order_relaxed);
    total_size += data_size;
  }

  // repeated .proto2_preserve_unknown_enum_unittest.MyEnum repeated_packed_unexpected_e = 4;
  {
    size_t data_size = 0;
    unsigned int count = static_cast<unsigned int>(this->repeated_packed_unexpected_e_size());for (unsigned int i = 0; i < count; i++) {
      data_size += ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::EnumSize(
        this->repeated_packed_unexpected_e(static_cast<int>(i)));
    }
    total_size += (1UL * count) + data_size;
  }

  // optional .proto2_preserve_unknown_enum_unittest.MyEnum e = 1;
  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x00000001u) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::EnumSize(this->e());
  }

  switch (o_case()) {
    // optional .proto2_preserve_unknown_enum_unittest.MyEnum oneof_e_1 = 5;
    case kOneofE1: {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::EnumSize(this->oneof_e_1());
      break;
    }
    // optional .proto2_preserve_unknown_enum_unittest.MyEnum oneof_e_2 = 6;
    case kOneofE2: {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::EnumSize(this->oneof_e_2());
      break;
    }
    case O_NOT_SET: {
      break;
    }
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void MyMessage::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:proto2_preserve_unknown_enum_unittest.MyMessage)
  GOOGLE_DCHECK_NE(&from, this);
  const MyMessage* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<MyMessage>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:proto2_preserve_unknown_enum_unittest.MyMessage)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:proto2_preserve_unknown_enum_unittest.MyMessage)
    MergeFrom(*source);
  }
}

void MyMessage::MergeFrom(const MyMessage& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:proto2_preserve_unknown_enum_unittest.MyMessage)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  repeated_e_.MergeFrom(from.repeated_e_);
  repeated_packed_e_.MergeFrom(from.repeated_packed_e_);
  repeated_packed_unexpected_e_.MergeFrom(from.repeated_packed_unexpected_e_);
  if (from.has_e()) {
    set_e(from.e());
  }
  switch (from.o_case()) {
    case kOneofE1: {
      set_oneof_e_1(from.oneof_e_1());
      break;
    }
    case kOneofE2: {
      set_oneof_e_2(from.oneof_e_2());
      break;
    }
    case O_NOT_SET: {
      break;
    }
  }
}

void MyMessage::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:proto2_preserve_unknown_enum_unittest.MyMessage)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void MyMessage::CopyFrom(const MyMessage& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:proto2_preserve_unknown_enum_unittest.MyMessage)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool MyMessage::IsInitialized() const {
  return true;
}

void MyMessage::Swap(MyMessage* other) {
  if (other == this) return;
  InternalSwap(other);
}
void MyMessage::InternalSwap(MyMessage* other) {
  using std::swap;
  _internal_metadata_.Swap(&other->_internal_metadata_);
  swap(_has_bits_[0], other->_has_bits_[0]);
  repeated_e_.InternalSwap(&other->repeated_e_);
  repeated_packed_e_.InternalSwap(&other->repeated_packed_e_);
  repeated_packed_unexpected_e_.InternalSwap(&other->repeated_packed_unexpected_e_);
  swap(e_, other->e_);
  swap(o_, other->o_);
  swap(_oneof_case_[0], other->_oneof_case_[0]);
}

::PROTOBUF_NAMESPACE_ID::Metadata MyMessage::GetMetadata() const {
  return GetMetadataStatic();
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace proto2_preserve_unknown_enum_unittest
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::proto2_preserve_unknown_enum_unittest::MyMessage* Arena::CreateMaybeMessage< ::proto2_preserve_unknown_enum_unittest::MyMessage >(Arena* arena) {
  return Arena::CreateInternal< ::proto2_preserve_unknown_enum_unittest::MyMessage >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
