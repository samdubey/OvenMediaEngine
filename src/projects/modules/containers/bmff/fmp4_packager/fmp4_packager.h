//==============================================================================
//
//  OvenMediaEngine
//
//  Created by Getroot
//  Copyright (c) 2022 AirenSoft. All rights reserved.
//
//==============================================================================
#pragma once

#include <base/info/media_track.h>
#include <base/mediarouter/media_buffer.h>

#include "../bmff_packager.h"
#include "fmp4_storage.h"

namespace bmff
{
	class FMP4Packager : public Packager
	{
	public:
		struct Config
		{
			uint32_t chunk_duration_ms = 1000;
		};

		FMP4Packager(const std::shared_ptr<FMP4Storage> &storage, const std::shared_ptr<const MediaTrack> &track, const Config &config);

		// Generate Initialization FMP4Segment
		bool CreateInitializationSegment();

		// Generate Media FMP4Segment
		bool AppendSample(const std::shared_ptr<const MediaPacket> &media_packet);

		// For MediaType::Data MediaPacket, It will be emplaced in emsg box.
		bool AppendDataSample(const std::shared_ptr<const MediaPacket> &media_packet);

	private:
		const Config &GetConfig() const;

		bool StoreInitializationSection(const std::shared_ptr<ov::Data> &segment);
		bool AppendMediaChunk(const std::shared_ptr<ov::Data> &chunk, uint64_t start_timestamp, uint32_t duration_ms, bool independent);

		std::shared_ptr<const MediaPacket> ConvertBitstreamFormat(const std::shared_ptr<const MediaPacket> &media_packet);

		bool WriteFtypBox(ov::ByteStream &data_stream) override;

		Config _config;
		std::shared_ptr<FMP4Storage> _storage = nullptr;
		std::shared_ptr<Samples> _samples_buffer = nullptr;
		std::shared_ptr<Samples> _data_samples_buffer = nullptr;
	};
}