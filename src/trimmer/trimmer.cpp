#include "trimmer.h"

#include <QDebug>

extern "C"
{
    #include <libavutil/opt.h>
}

Trimmer::Trimmer(QObject* parent) : QThread(parent)
{
    inputContext = NULL;
    inputVideoStream = NULL;
    outputContext = NULL;

    inPTS = -1;
    outPTS = -1;
}

Trimmer::~Trimmer()
{
}

void Trimmer::run()
{
    int videoTB = inputVideoStream == NULL ? 0 : av_q2d(inputVideoStream->time_base) * 1000;
    bool canOutput = false;

    while(true)
    {
        AVPacket packet = { 0 };

        if(av_read_frame(inputContext, &packet) >= 0)
        {
            if(inputVideoStream != NULL && packet.stream_index == inputVideoStream->index)
            {
                int64_t pts = 0;

                if(packet.dts != AV_NOPTS_VALUE)
                    pts = packet.dts;
                else if(packet.pts != AV_NOPTS_VALUE)
                    pts = packet.pts;

                pts -= inputContext->start_time;
                pts *= videoTB;

                if(outPTS <= pts)
                    break;

                emit trimProgress(pts);

                if(inPTS <= pts)
                    canOutput = true;
            }

            if(canOutput)
                av_interleaved_write_frame(outputContext, &packet);
            av_packet_unref(&packet);
        }
        else break;
    }

    emit trimProgress(outPTS);

    // Write the muxer trailer
    av_write_trailer(outputContext);

    cleanup();
}

bool Trimmer::initTrimmer(const QString& inputFile, int inPTS, int outPTS, const QString& outputFile, const char* timecode)
{
    if(inPTS < 0 || outPTS < 0)
        return false;

    this->inPTS = inPTS;
    this->outPTS = outPTS;

    // Open source video file
    if(avformat_open_input(&inputContext, inputFile.toStdString().c_str(), NULL, NULL) != 0)
    {
        cleanup();
        return false;
    }

    if(avformat_find_stream_info(inputContext, NULL) < 0)
    {
        cleanup();
        return false;
    }

    // TODO: only working for mxf_d10, add more formats
    if(avformat_alloc_output_context2(&outputContext, NULL, "mxf_d10", outputFile.toStdString().c_str()) < 0)
    {
        cleanup();
        return false;
    }

    av_dict_set(&outputContext->metadata, "timecode", timecode, 0);

    for(unsigned int i=0; i < inputContext->nb_streams; i++)
    {
        AVCodecParameters* codecParameters = inputContext->streams[i]->codecpar;
        AVMediaType codecType = codecParameters->codec_type;

        if(codecType == AVMEDIA_TYPE_VIDEO || codecType == AVMEDIA_TYPE_AUDIO)
        {
            if(codecType == AVMEDIA_TYPE_VIDEO && inputVideoStream == NULL)
                inputVideoStream = inputContext->streams[i];

            AVCodec* codec = avcodec_find_decoder(codecParameters->codec_id);
            if(codec == NULL)
            {
                cleanup();
                qDebug() << "Problem codec == NULL";
                return false;
            }

            AVCodecContext* codecContext = avcodec_alloc_context3(codec);
            avcodec_parameters_to_context(codecContext, codecParameters);

            // Open codec
            if(avcodec_open2(codecContext, codec, NULL) < 0)
            {
                cleanup();
                qDebug() << "Problem avcodec_open2()";
                return false;
            }

            // Create new stream for output and copy its context from the input
            AVStream* stream = avformat_new_stream(outputContext, codec);
            if(avcodec_parameters_copy(stream->codecpar, codecParameters) != 0)
            {
                cleanup();
                qDebug() << "Problem avcodec_copy_context()";
                return false;
            }

            // Define stream id, copy timebase and aspect ratio from input
            stream->id = outputContext->nb_streams-1;
            codecContext->time_base = stream->time_base = inputContext->streams[i]->time_base;
            stream->codecpar->sample_aspect_ratio = codecContext->sample_aspect_ratio;
            stream->sample_aspect_ratio = inputContext->streams[i]->sample_aspect_ratio;

            // Open codec
            if(avcodec_open2(codecContext, codec, NULL) < 0)
            {
                cleanup();
                qDebug() << "Problem avcodec_open2()";
                return false;
            }
        }
    }

    // Open the output file, if needed
    if(!(outputContext->oformat->flags & AVFMT_NOFILE) && (avio_open(&outputContext->pb, outputFile.toStdString().c_str(), AVIO_FLAG_WRITE) < 0))
    {
        cleanup();
        return false;
    }

    // Write the muxer header
    if(avformat_write_header(outputContext, NULL) < 0)
    {
        cleanup();
        return false;
    }

    //av_dump_format(inputContext, 0, inputFile.toStdString().c_str(), 0);
    //av_dump_format(outputContext, 0, outputFile.toStdString().c_str(), 1);

    return true;
}

void Trimmer::cleanup()
{
    if(outputContext != NULL)
    {
        for(unsigned int i=0; i < outputContext->nb_streams; i++)
        {
            AVCodecParameters* codecParameters = inputContext->streams[i]->codecpar;
            AVMediaType codecType = codecParameters->codec_type;

            if(codecType == AVMEDIA_TYPE_VIDEO || codecType == AVMEDIA_TYPE_AUDIO)
            {
                avcodec_parameters_free(&codecParameters);
            }
        }

        // Close the output file
        if(outputContext->pb != NULL)
            avio_close(outputContext->pb);

        // Free the context and its streams
        avformat_free_context(outputContext);
    }
    outputContext = NULL;

    if(inputContext != NULL)
    {
        for(unsigned int i=0; i < inputContext->nb_streams; i++)
        {
            AVCodecParameters* codecParameters = inputContext->streams[i]->codecpar;
            AVMediaType codecType = codecParameters->codec_type;

            if(codecType == AVMEDIA_TYPE_VIDEO || codecType == AVMEDIA_TYPE_AUDIO)
            {
                avcodec_parameters_free(&codecParameters);
            }
        }

        avformat_close_input(&outputContext);
        avformat_free_context(outputContext);
    }
    inputVideoStream = NULL;
    inputContext = NULL;

    inPTS = -1;
    outPTS = -1;
}
