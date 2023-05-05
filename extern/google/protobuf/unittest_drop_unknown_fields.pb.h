// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: google/protobuf/unittest_drop_unknown_fields.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3008000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3008000 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[2]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto;
namespace unittest_drop_unknown_fields {
class Foo;
class FooDefaultTypeInternal;
extern FooDefaultTypeInternal _Foo_default_instance_;
class FooWithExtraFields;
class FooWithExtraFieldsDefaultTypeInternal;
extern FooWithExtraFieldsDefaultTypeInternal _FooWithExtraFields_default_instance_;
}  // namespace unittest_drop_unknown_fields
PROTOBUF_NAMESPACE_OPEN
template<> ::unittest_drop_unknown_fields::Foo* Arena::CreateMaybeMessage<::unittest_drop_unknown_fields::Foo>(Arena*);
template<> ::unittest_drop_unknown_fields::FooWithExtraFields* Arena::CreateMaybeMessage<::unittest_drop_unknown_fields::FooWithExtraFields>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace unittest_drop_unknown_fields {

enum Foo_NestedEnum : int {
  Foo_NestedEnum_FOO = 0,
  Foo_NestedEnum_BAR = 1,
  Foo_NestedEnum_BAZ = 2,
  Foo_NestedEnum_Foo_NestedEnum_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  Foo_NestedEnum_Foo_NestedEnum_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool Foo_NestedEnum_IsValid(int value);
constexpr Foo_NestedEnum Foo_NestedEnum_NestedEnum_MIN = Foo_NestedEnum_FOO;
constexpr Foo_NestedEnum Foo_NestedEnum_NestedEnum_MAX = Foo_NestedEnum_BAZ;
constexpr int Foo_NestedEnum_NestedEnum_ARRAYSIZE = Foo_NestedEnum_NestedEnum_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* Foo_NestedEnum_descriptor();
template<typename T>
inline const std::string& Foo_NestedEnum_Name(T enum_t_value) {
  static_assert(::std::is_same<T, Foo_NestedEnum>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function Foo_NestedEnum_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    Foo_NestedEnum_descriptor(), enum_t_value);
}
inline bool Foo_NestedEnum_Parse(
    const std::string& name, Foo_NestedEnum* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<Foo_NestedEnum>(
    Foo_NestedEnum_descriptor(), name, value);
}
enum FooWithExtraFields_NestedEnum : int {
  FooWithExtraFields_NestedEnum_FOO = 0,
  FooWithExtraFields_NestedEnum_BAR = 1,
  FooWithExtraFields_NestedEnum_BAZ = 2,
  FooWithExtraFields_NestedEnum_QUX = 3,
  FooWithExtraFields_NestedEnum_FooWithExtraFields_NestedEnum_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  FooWithExtraFields_NestedEnum_FooWithExtraFields_NestedEnum_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool FooWithExtraFields_NestedEnum_IsValid(int value);
constexpr FooWithExtraFields_NestedEnum FooWithExtraFields_NestedEnum_NestedEnum_MIN = FooWithExtraFields_NestedEnum_FOO;
constexpr FooWithExtraFields_NestedEnum FooWithExtraFields_NestedEnum_NestedEnum_MAX = FooWithExtraFields_NestedEnum_QUX;
constexpr int FooWithExtraFields_NestedEnum_NestedEnum_ARRAYSIZE = FooWithExtraFields_NestedEnum_NestedEnum_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* FooWithExtraFields_NestedEnum_descriptor();
template<typename T>
inline const std::string& FooWithExtraFields_NestedEnum_Name(T enum_t_value) {
  static_assert(::std::is_same<T, FooWithExtraFields_NestedEnum>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function FooWithExtraFields_NestedEnum_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    FooWithExtraFields_NestedEnum_descriptor(), enum_t_value);
}
inline bool FooWithExtraFields_NestedEnum_Parse(
    const std::string& name, FooWithExtraFields_NestedEnum* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<FooWithExtraFields_NestedEnum>(
    FooWithExtraFields_NestedEnum_descriptor(), name, value);
}
// ===================================================================

class Foo :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:unittest_drop_unknown_fields.Foo) */ {
 public:
  Foo();
  virtual ~Foo();

  Foo(const Foo& from);
  Foo(Foo&& from) noexcept
    : Foo() {
    *this = ::std::move(from);
  }

  inline Foo& operator=(const Foo& from) {
    CopyFrom(from);
    return *this;
  }
  inline Foo& operator=(Foo&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const Foo& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Foo* internal_default_instance() {
    return reinterpret_cast<const Foo*>(
               &_Foo_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  void Swap(Foo* other);
  friend void swap(Foo& a, Foo& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline Foo* New() const final {
    return CreateMaybeMessage<Foo>(nullptr);
  }

  Foo* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<Foo>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const Foo& from);
  void MergeFrom(const Foo& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  #if GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  #else
  bool MergePartialFromCodedStream(
      ::PROTOBUF_NAMESPACE_ID::io::CodedInputStream* input) final;
  #endif  // GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
  void SerializeWithCachedSizes(
      ::PROTOBUF_NAMESPACE_ID::io::CodedOutputStream* output) const final;
  ::PROTOBUF_NAMESPACE_ID::uint8* InternalSerializeWithCachedSizesToArray(
      ::PROTOBUF_NAMESPACE_ID::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Foo* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "unittest_drop_unknown_fields.Foo";
  }
  private:
  inline ::PROTOBUF_NAMESPACE_ID::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto);
    return ::descriptor_table_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  typedef Foo_NestedEnum NestedEnum;
  static constexpr NestedEnum FOO =
    Foo_NestedEnum_FOO;
  static constexpr NestedEnum BAR =
    Foo_NestedEnum_BAR;
  static constexpr NestedEnum BAZ =
    Foo_NestedEnum_BAZ;
  static inline bool NestedEnum_IsValid(int value) {
    return Foo_NestedEnum_IsValid(value);
  }
  static constexpr NestedEnum NestedEnum_MIN =
    Foo_NestedEnum_NestedEnum_MIN;
  static constexpr NestedEnum NestedEnum_MAX =
    Foo_NestedEnum_NestedEnum_MAX;
  static constexpr int NestedEnum_ARRAYSIZE =
    Foo_NestedEnum_NestedEnum_ARRAYSIZE;
  static inline const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor*
  NestedEnum_descriptor() {
    return Foo_NestedEnum_descriptor();
  }
  template<typename T>
  static inline const std::string& NestedEnum_Name(T enum_t_value) {
    static_assert(::std::is_same<T, NestedEnum>::value ||
      ::std::is_integral<T>::value,
      "Incorrect type passed to function NestedEnum_Name.");
    return Foo_NestedEnum_Name(enum_t_value);
  }
  static inline bool NestedEnum_Parse(const std::string& name,
      NestedEnum* value) {
    return Foo_NestedEnum_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  // int32 int32_value = 1;
  void clear_int32_value();
  static const int kInt32ValueFieldNumber = 1;
  ::PROTOBUF_NAMESPACE_ID::int32 int32_value() const;
  void set_int32_value(::PROTOBUF_NAMESPACE_ID::int32 value);

  // .unittest_drop_unknown_fields.Foo.NestedEnum enum_value = 2;
  void clear_enum_value();
  static const int kEnumValueFieldNumber = 2;
  ::unittest_drop_unknown_fields::Foo_NestedEnum enum_value() const;
  void set_enum_value(::unittest_drop_unknown_fields::Foo_NestedEnum value);

  // @@protoc_insertion_point(class_scope:unittest_drop_unknown_fields.Foo)
 private:
  class HasBitSetters;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::int32 int32_value_;
  int enum_value_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto;
};
// -------------------------------------------------------------------

class FooWithExtraFields :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:unittest_drop_unknown_fields.FooWithExtraFields) */ {
 public:
  FooWithExtraFields();
  virtual ~FooWithExtraFields();

  FooWithExtraFields(const FooWithExtraFields& from);
  FooWithExtraFields(FooWithExtraFields&& from) noexcept
    : FooWithExtraFields() {
    *this = ::std::move(from);
  }

  inline FooWithExtraFields& operator=(const FooWithExtraFields& from) {
    CopyFrom(from);
    return *this;
  }
  inline FooWithExtraFields& operator=(FooWithExtraFields&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const FooWithExtraFields& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const FooWithExtraFields* internal_default_instance() {
    return reinterpret_cast<const FooWithExtraFields*>(
               &_FooWithExtraFields_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  void Swap(FooWithExtraFields* other);
  friend void swap(FooWithExtraFields& a, FooWithExtraFields& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline FooWithExtraFields* New() const final {
    return CreateMaybeMessage<FooWithExtraFields>(nullptr);
  }

  FooWithExtraFields* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<FooWithExtraFields>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const FooWithExtraFields& from);
  void MergeFrom(const FooWithExtraFields& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  #if GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  #else
  bool MergePartialFromCodedStream(
      ::PROTOBUF_NAMESPACE_ID::io::CodedInputStream* input) final;
  #endif  // GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
  void SerializeWithCachedSizes(
      ::PROTOBUF_NAMESPACE_ID::io::CodedOutputStream* output) const final;
  ::PROTOBUF_NAMESPACE_ID::uint8* InternalSerializeWithCachedSizesToArray(
      ::PROTOBUF_NAMESPACE_ID::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(FooWithExtraFields* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "unittest_drop_unknown_fields.FooWithExtraFields";
  }
  private:
  inline ::PROTOBUF_NAMESPACE_ID::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto);
    return ::descriptor_table_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  typedef FooWithExtraFields_NestedEnum NestedEnum;
  static constexpr NestedEnum FOO =
    FooWithExtraFields_NestedEnum_FOO;
  static constexpr NestedEnum BAR =
    FooWithExtraFields_NestedEnum_BAR;
  static constexpr NestedEnum BAZ =
    FooWithExtraFields_NestedEnum_BAZ;
  static constexpr NestedEnum QUX =
    FooWithExtraFields_NestedEnum_QUX;
  static inline bool NestedEnum_IsValid(int value) {
    return FooWithExtraFields_NestedEnum_IsValid(value);
  }
  static constexpr NestedEnum NestedEnum_MIN =
    FooWithExtraFields_NestedEnum_NestedEnum_MIN;
  static constexpr NestedEnum NestedEnum_MAX =
    FooWithExtraFields_NestedEnum_NestedEnum_MAX;
  static constexpr int NestedEnum_ARRAYSIZE =
    FooWithExtraFields_NestedEnum_NestedEnum_ARRAYSIZE;
  static inline const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor*
  NestedEnum_descriptor() {
    return FooWithExtraFields_NestedEnum_descriptor();
  }
  template<typename T>
  static inline const std::string& NestedEnum_Name(T enum_t_value) {
    static_assert(::std::is_same<T, NestedEnum>::value ||
      ::std::is_integral<T>::value,
      "Incorrect type passed to function NestedEnum_Name.");
    return FooWithExtraFields_NestedEnum_Name(enum_t_value);
  }
  static inline bool NestedEnum_Parse(const std::string& name,
      NestedEnum* value) {
    return FooWithExtraFields_NestedEnum_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  // int32 int32_value = 1;
  void clear_int32_value();
  static const int kInt32ValueFieldNumber = 1;
  ::PROTOBUF_NAMESPACE_ID::int32 int32_value() const;
  void set_int32_value(::PROTOBUF_NAMESPACE_ID::int32 value);

  // .unittest_drop_unknown_fields.FooWithExtraFields.NestedEnum enum_value = 2;
  void clear_enum_value();
  static const int kEnumValueFieldNumber = 2;
  ::unittest_drop_unknown_fields::FooWithExtraFields_NestedEnum enum_value() const;
  void set_enum_value(::unittest_drop_unknown_fields::FooWithExtraFields_NestedEnum value);

  // int32 extra_int32_value = 3;
  void clear_extra_int32_value();
  static const int kExtraInt32ValueFieldNumber = 3;
  ::PROTOBUF_NAMESPACE_ID::int32 extra_int32_value() const;
  void set_extra_int32_value(::PROTOBUF_NAMESPACE_ID::int32 value);

  // @@protoc_insertion_point(class_scope:unittest_drop_unknown_fields.FooWithExtraFields)
 private:
  class HasBitSetters;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::int32 int32_value_;
  int enum_value_;
  ::PROTOBUF_NAMESPACE_ID::int32 extra_int32_value_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// Foo

// int32 int32_value = 1;
inline void Foo::clear_int32_value() {
  int32_value_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Foo::int32_value() const {
  // @@protoc_insertion_point(field_get:unittest_drop_unknown_fields.Foo.int32_value)
  return int32_value_;
}
inline void Foo::set_int32_value(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  int32_value_ = value;
  // @@protoc_insertion_point(field_set:unittest_drop_unknown_fields.Foo.int32_value)
}

// .unittest_drop_unknown_fields.Foo.NestedEnum enum_value = 2;
inline void Foo::clear_enum_value() {
  enum_value_ = 0;
}
inline ::unittest_drop_unknown_fields::Foo_NestedEnum Foo::enum_value() const {
  // @@protoc_insertion_point(field_get:unittest_drop_unknown_fields.Foo.enum_value)
  return static_cast< ::unittest_drop_unknown_fields::Foo_NestedEnum >(enum_value_);
}
inline void Foo::set_enum_value(::unittest_drop_unknown_fields::Foo_NestedEnum value) {
  
  enum_value_ = value;
  // @@protoc_insertion_point(field_set:unittest_drop_unknown_fields.Foo.enum_value)
}

// -------------------------------------------------------------------

// FooWithExtraFields

// int32 int32_value = 1;
inline void FooWithExtraFields::clear_int32_value() {
  int32_value_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 FooWithExtraFields::int32_value() const {
  // @@protoc_insertion_point(field_get:unittest_drop_unknown_fields.FooWithExtraFields.int32_value)
  return int32_value_;
}
inline void FooWithExtraFields::set_int32_value(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  int32_value_ = value;
  // @@protoc_insertion_point(field_set:unittest_drop_unknown_fields.FooWithExtraFields.int32_value)
}

// .unittest_drop_unknown_fields.FooWithExtraFields.NestedEnum enum_value = 2;
inline void FooWithExtraFields::clear_enum_value() {
  enum_value_ = 0;
}
inline ::unittest_drop_unknown_fields::FooWithExtraFields_NestedEnum FooWithExtraFields::enum_value() const {
  // @@protoc_insertion_point(field_get:unittest_drop_unknown_fields.FooWithExtraFields.enum_value)
  return static_cast< ::unittest_drop_unknown_fields::FooWithExtraFields_NestedEnum >(enum_value_);
}
inline void FooWithExtraFields::set_enum_value(::unittest_drop_unknown_fields::FooWithExtraFields_NestedEnum value) {
  
  enum_value_ = value;
  // @@protoc_insertion_point(field_set:unittest_drop_unknown_fields.FooWithExtraFields.enum_value)
}

// int32 extra_int32_value = 3;
inline void FooWithExtraFields::clear_extra_int32_value() {
  extra_int32_value_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 FooWithExtraFields::extra_int32_value() const {
  // @@protoc_insertion_point(field_get:unittest_drop_unknown_fields.FooWithExtraFields.extra_int32_value)
  return extra_int32_value_;
}
inline void FooWithExtraFields::set_extra_int32_value(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  extra_int32_value_ = value;
  // @@protoc_insertion_point(field_set:unittest_drop_unknown_fields.FooWithExtraFields.extra_int32_value)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace unittest_drop_unknown_fields

PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::unittest_drop_unknown_fields::Foo_NestedEnum> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::unittest_drop_unknown_fields::Foo_NestedEnum>() {
  return ::unittest_drop_unknown_fields::Foo_NestedEnum_descriptor();
}
template <> struct is_proto_enum< ::unittest_drop_unknown_fields::FooWithExtraFields_NestedEnum> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::unittest_drop_unknown_fields::FooWithExtraFields_NestedEnum>() {
  return ::unittest_drop_unknown_fields::FooWithExtraFields_NestedEnum_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto
