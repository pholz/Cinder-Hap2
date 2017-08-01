/*
 *  MovieHap.h
 *
 *  Created by Roger Sodre on 14/05/10.
 *  Copyright 2010 Studio Avante. All rights reserved.
 *
 */
#pragma once

#include "cinder/Cinder.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"

#if defined( CINDER_MSW )
#define _STDINT_H
#define __FP__
#endif
#include "cinder/qtime/QuicktimeGl.h"

namespace cinder { namespace qtime {
	
	typedef std::shared_ptr<class MovieGlHap> MovieGlHapRef;
	
	class MovieGlHap : public MovieBase {
	public:
		enum class Codec { HAP, HAP_A, HAP_Q, UNSUPPORTED };
		
		~MovieGlHap();
		MovieGlHap( const fs::path &path );
		MovieGlHap( const class MovieLoader &loader );
		MovieGlHap( const void *data, size_t dataSize, const std::string &fileNameHint, const std::string &mimeTypeHint = "" );
		MovieGlHap( DataSourceRef dataSource, const std::string mimeTypeHint = "" );
		
		gl::Texture2dRef getTexture();
		gl::GlslProgRef getGlsl() const;
		void draw();
		
		bool			isHap() const { return mCodec == Codec::HAP; }
		bool			isHapA() const { return mCodec == Codec::HAP_A; }
		bool			isHapQ() const { return mCodec == Codec::HAP_Q; }
		
		const Codec&	getCodecName() const { return mCodec; }
		float			getPlaybackFramerate() const;
		
		static MovieGlHapRef create( const fs::path &path ) { return MovieGlHapRef( new MovieGlHap( path ) ); }
		static MovieGlHapRef create( const MovieLoaderRef &loader );
		static MovieGlHapRef create( const void *data, size_t dataSize, const std::string &fileNameHint, const std::string &mimeTypeHint = "" )
		{ return MovieGlHapRef( new MovieGlHap( data, dataSize, fileNameHint, mimeTypeHint ) ); }
		static MovieGlHapRef create( DataSourceRef dataSource, const std::string mimeTypeHint = "" )
		{ return MovieGlHapRef( new MovieGlHap( dataSource, mimeTypeHint ) ); }

		float contentWidth() { return mObj->contentWidth; };
		float contentHeight() { return mObj->contentHeight; };

	protected:
		
		void allocateVisualContext();

		struct Obj : public MovieBase::Obj {
			Obj();
			~Obj();
			virtual void		releaseFrame();
			virtual void		newFrame( CVImageBufferRef cvImage );
			gl::Texture2dRef	mTexture;
			gl::GlslProgRef		mDefaultShader;
			static gl::GlslProgRef	sHapQShader;
			std::once_flag			mHapQOnceFlag;
			float contentWidth, contentHeight;
		};
		std::unique_ptr<Obj>		mObj;
		virtual MovieBase::Obj*		getObj() const { return mObj.get(); }
		
		Codec						mCodec;
	};

} } //namespace cinder::qtime
