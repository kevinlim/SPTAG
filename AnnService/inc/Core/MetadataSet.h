// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef _SPTAG_METADATASET_H_
#define _SPTAG_METADATASET_H_

#include "CommonDataStructure.h"

#include <iostream>
#include <fstream>

namespace SPTAG
{

class MetadataSet
{
public:
    MetadataSet();

    virtual ~MetadataSet();

    virtual ByteArray GetMetadata(SizeType p_vectorID) const = 0;

    virtual SizeType Count() const = 0;

    virtual bool Available() const = 0;

    virtual std::pair<std::uint64_t, std::uint64_t> BufferSize() const = 0;

    virtual void Add(const ByteArray& data) = 0;

    virtual ErrorCode SaveMetadata(std::ostream& p_metaOut, std::ostream& p_metaIndexOut) = 0;

    virtual ErrorCode SaveMetadata(const std::string& p_metaFile, const std::string& p_metaindexFile) = 0;
 
    virtual void AddBatch(MetadataSet& data);
    
    virtual ErrorCode RefineMetadata(std::vector<SizeType>& indices, std::shared_ptr<MetadataSet>& p_newMetadata) const;

    virtual ErrorCode RefineMetadata(std::vector<SizeType>& indices, std::ostream& p_metaOut, std::ostream& p_metaIndexOut) const;

    virtual ErrorCode RefineMetadata(std::vector<SizeType>& indices, const std::string& p_metaFile, const std::string& p_metaindexFile) const;
};


class FileMetadataSet : public MetadataSet
{
public:
    FileMetadataSet(const std::string& p_metaFile, const std::string& p_metaindexFile);
    
    ~FileMetadataSet();

    ByteArray GetMetadata(SizeType p_vectorID) const;

    SizeType Count() const;

    bool Available() const;

    std::pair<std::uint64_t, std::uint64_t> BufferSize() const;
    
    void Add(const ByteArray& data);

    ErrorCode SaveMetadata(std::ostream& p_metaOut, std::ostream& p_metaIndexOut);

    ErrorCode SaveMetadata(const std::string& p_metaFile, const std::string& p_metaindexFile);

private:
    std::ifstream* m_fp = nullptr;

    std::vector<std::uint64_t> m_pOffsets;

    SizeType m_count;

    std::string m_metaFile;

    std::string m_metaindexFile;

    std::vector<std::uint8_t> m_newdata;
};


class MemMetadataSet : public MetadataSet
{
public:
    MemMetadataSet();

    MemMetadataSet(ByteArray p_metadata, ByteArray p_offsets, SizeType p_count);

    MemMetadataSet(const std::string& p_metafile, const std::string& p_metaindexfile);

    MemMetadataSet(std::istream& p_metain, std::istream& p_metaindexin);

    ~MemMetadataSet();

    ByteArray GetMetadata(SizeType p_vectorID) const;

    SizeType Count() const;

    bool Available() const;

    std::pair<std::uint64_t, std::uint64_t> BufferSize() const;

    void Add(const ByteArray& data);

    ErrorCode SaveMetadata(std::ostream& p_metaOut, std::ostream& p_metaIndexOut);

    ErrorCode SaveMetadata(const std::string& p_metaFile, const std::string& p_metaindexFile);

private:
    void Init(std::istream& p_metain, std::istream& p_metaindexin);

    std::vector<std::uint64_t> m_offsets;

    SizeType m_count;

    ByteArray m_metadataHolder;

    std::vector<std::uint8_t> m_newdata;
};


} // namespace SPTAG

#endif // _SPTAG_METADATASET_H_
