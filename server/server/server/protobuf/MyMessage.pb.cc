// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: MyMessage.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "MyMessage.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace net_protocol {

namespace {

const ::google::protobuf::Descriptor* LogonReqMessage_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  LogonReqMessage_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_MyMessage_2eproto() {
  protobuf_AddDesc_MyMessage_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "MyMessage.proto");
  GOOGLE_CHECK(file != NULL);
  LogonReqMessage_descriptor_ = file->message_type(0);
  static const int LogonReqMessage_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(LogonReqMessage, acctid_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(LogonReqMessage, passwd_),
  };
  LogonReqMessage_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      LogonReqMessage_descriptor_,
      LogonReqMessage::default_instance_,
      LogonReqMessage_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(LogonReqMessage, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(LogonReqMessage, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(LogonReqMessage));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_MyMessage_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    LogonReqMessage_descriptor_, &LogonReqMessage::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_MyMessage_2eproto() {
  delete LogonReqMessage::default_instance_;
  delete LogonReqMessage_reflection_;
}

void protobuf_AddDesc_MyMessage_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\017MyMessage.proto\022\014net_protocol\"1\n\017Logon"
    "ReqMessage\022\016\n\006acctID\030\001 \002(\003\022\016\n\006passwd\030\002 \002"
    "(\t", 82);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "MyMessage.proto", &protobuf_RegisterTypes);
  LogonReqMessage::default_instance_ = new LogonReqMessage();
  LogonReqMessage::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_MyMessage_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_MyMessage_2eproto {
  StaticDescriptorInitializer_MyMessage_2eproto() {
    protobuf_AddDesc_MyMessage_2eproto();
  }
} static_descriptor_initializer_MyMessage_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int LogonReqMessage::kAcctIDFieldNumber;
const int LogonReqMessage::kPasswdFieldNumber;
#endif  // !_MSC_VER

LogonReqMessage::LogonReqMessage()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void LogonReqMessage::InitAsDefaultInstance() {
}

LogonReqMessage::LogonReqMessage(const LogonReqMessage& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void LogonReqMessage::SharedCtor() {
  _cached_size_ = 0;
  acctid_ = GOOGLE_LONGLONG(0);
  passwd_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

LogonReqMessage::~LogonReqMessage() {
  SharedDtor();
}

void LogonReqMessage::SharedDtor() {
  if (passwd_ != &::google::protobuf::internal::kEmptyString) {
    delete passwd_;
  }
  if (this != default_instance_) {
  }
}

void LogonReqMessage::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* LogonReqMessage::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return LogonReqMessage_descriptor_;
}

const LogonReqMessage& LogonReqMessage::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_MyMessage_2eproto();
  return *default_instance_;
}

LogonReqMessage* LogonReqMessage::default_instance_ = NULL;

LogonReqMessage* LogonReqMessage::New() const {
  return new LogonReqMessage;
}

void LogonReqMessage::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    acctid_ = GOOGLE_LONGLONG(0);
    if (has_passwd()) {
      if (passwd_ != &::google::protobuf::internal::kEmptyString) {
        passwd_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool LogonReqMessage::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required int64 acctID = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int64, ::google::protobuf::internal::WireFormatLite::TYPE_INT64>(
                 input, &acctid_)));
          set_has_acctid();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_passwd;
        break;
      }

      // required string passwd = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_passwd:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_passwd()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->passwd().data(), this->passwd().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void LogonReqMessage::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required int64 acctID = 1;
  if (has_acctid()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt64(1, this->acctid(), output);
  }

  // required string passwd = 2;
  if (has_passwd()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->passwd().data(), this->passwd().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      2, this->passwd(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* LogonReqMessage::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required int64 acctID = 1;
  if (has_acctid()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt64ToArray(1, this->acctid(), target);
  }

  // required string passwd = 2;
  if (has_passwd()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->passwd().data(), this->passwd().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        2, this->passwd(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int LogonReqMessage::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required int64 acctID = 1;
    if (has_acctid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int64Size(
          this->acctid());
    }

    // required string passwd = 2;
    if (has_passwd()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->passwd());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void LogonReqMessage::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const LogonReqMessage* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const LogonReqMessage*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void LogonReqMessage::MergeFrom(const LogonReqMessage& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_acctid()) {
      set_acctid(from.acctid());
    }
    if (from.has_passwd()) {
      set_passwd(from.passwd());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void LogonReqMessage::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void LogonReqMessage::CopyFrom(const LogonReqMessage& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool LogonReqMessage::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;

  return true;
}

void LogonReqMessage::Swap(LogonReqMessage* other) {
  if (other != this) {
    std::swap(acctid_, other->acctid_);
    std::swap(passwd_, other->passwd_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata LogonReqMessage::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = LogonReqMessage_descriptor_;
  metadata.reflection = LogonReqMessage_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace net_protocol

// @@protoc_insertion_point(global_scope)
