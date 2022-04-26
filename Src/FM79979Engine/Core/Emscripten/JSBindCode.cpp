#ifdef WASM
#include <emscripten.h>
#include <string>
//webgl fullscreen.
//https://webglfundamentals.org/webgl/lessons/webgl-resizing-the-canvas.html
//for mobile viewport
//https://impactjs.com/documentation/impact-on-mobile-platforms#always-render-in-the-native-resolution

//https://qiita.com/nokotan/items/35bea8b895eb7c9682de
//EM_JS
//(char*, prompt_name, (), 
//	{
//		var result = prompt('Please input your name', '');
//		var stringPtr = allocate(intArrayFromString(result || ''), 'i8', ALLOC_NORMAL);
//		return stringPtr;
//	}
//);

//
//
//EM_JS(void, prompt_name, (char* ptr, size_t length), {
//  var result = prompt('Please input your name', '');
//  stringToUTF8(result, ptr, length);
//});
//
//int main() {
//  char userName[100] = {};
//  prompt_name(userName, sizeof(userName));
//  printf("User Name: %s\n", userName);
//  return 0;
//}
//
//int main() 
//{
//  char* userName = prompt_name();
//  printf("User Name: %s\n", userName);
//  free(userName);
//  return 0;
//}




EM_JS
(
	char*, WASM_DodecodeURIComponent, (const char*e_str),
	{
		return decodeURIComponent(e_str);
	}
);



EM_JS
(
	void, WASM_JSInit, (),
	{
		canvas.onfullscreenchange = (event) =>
		{
			canvas.width = canvas.clientWidth;
			canvas.height = canvas.clientHeight;
		};
	}
);

EM_JS
(
	void, WASM_Fullscreen, (),
	{
		//return window.innerWidth;
		if (!document.fullscreenElement) 
		{
			  document.documentElement.requestFullscreen();
		}
		else
		{
			if (document.exitFullscreen)
			{
				document.exitFullscreen();
			}
		}
	}
	
);

//https://stackoverflow.com/questions/3437786/get-the-size-of-the-screen-current-web-page-and-browser-window
EM_JS
(//for portrait not landscape
	int, WASM_GetBrowserWidth, (),
	{
		return window.innerWidth;
		//return window.innerHeight;
	}
);

EM_JS
(//for portrait not landscape
	int, WASM_GetBrowserHeight, (),
	{
		//return window.innerWidth;
		return window.innerHeight;
	}
);

EM_JS
(
	void, WASM_JSViewportUpdate, (),
	{
		if (canvas.width != window.innerWidth-20)
		{
			canvas.width = window.innerWidth-20;
		}

		if (canvas.height != window.innerHeight-30)
		{
			canvas.height = window.innerHeight-30;
		}
	}
);

//https://stackoverflow.com/questions/36859325/fullscreen-full-dpi-webgl-canvas-on-mobile-device
EM_JS
(
	int, WASM_GetViewportWidth, (),
	{
		return canvas.width;
	}
);

EM_JS
(//for portrait not landscape
	int, WASM_GetViewportHeight, (),
	{
		return canvas.height;
	}
);

//https://stackify.dev/675027-proper-way-to-handle-sdl2-resizing-in-emscripten
//https://www.tutorialrepublic.com/faq/how-to-detect-screen-resolution-with-javascript.php#:~:text=Answer%3A%20Use%20the%20window.,on%20click%20of%20the%20button.
//https://stackoverflow.com/questions/1038727/how-to-get-browser-width-using-javascript-code


//https://stackoverflow.com/questions/11381673/detecting-a-mobile-browser
EM_JS
(
	bool, WASM_IsPC, (),
	{
		//from egret
		//let ua = window.navigator.userAgent.toLowerCase();
		//// console.log("userAgent = " + ua);
		//return !(/ android | adr / i.test(ua) || /ios | iphone | ipad | ipod | iwatch / i.test(ua));
		// 
		//let check = false;
		//(function(a) { if (/ (android | bb\d + | meego). + mobile | avantgo | bada\/|blackberry | blazer | compal | elaine | fennec | hiptop | iemobile | ip(hone | od) | iris | kindle | lge | maemo | midp | mmp | mobile. + firefox | netfront | opera m(ob | in)i | palm(os) ? | phone | p(ixi | re)\/|plucker | pocket | psp | series(4 | 6)0 | symbian | treo | up\.(browser | link) | vodafone | wap | windows ce | xda | xiino | android | ipad | playbook | silk / i.test(a)||/1207 | 6310 | 6590 | 3gso | 4thp | 50[1 - 6]i | 770s | 802s | a wa | abac | ac(er | oo | s\ - ) | ai(ko | rn) | al(av | ca | co) | amoi | an(ex | ny | yw) | aptu | ar(ch | go) | as(te | us) | attw | au(di | \ - m | r | s) | avan | be(ck | ll | nq) | bi(lb | rd) | bl(ac | az) | br(e | v)w | bumb | bw\ - (n | u) | c55\/|capi | ccwa | cdm\ - | cell | chtm | cldc | cmd\ - | co(mp | nd) | craw | da(it | ll | ng) | dbte | dc\ - s | devi | dica | dmob | do(c | p)o | ds(12 | \ - d) | el(49 | ai) | em(l2 | ul) | er(ic | k0) | esl8 | ez([4 - 7]0 | os | wa | ze) | fetc | fly(\ - | _) | g1 u | g560 | gene | gf\ - 5 | g\ - mo | go(\.w | od) | gr(ad | un) | haie | hcit | hd\ - (m | p | t) | hei\ - | hi(pt | ta) | hp(i | ip) | hs\ - c | ht(c(\ - | |_ | a | g | p | s | t) | tp) | hu(aw | tc) | i\ - (20 | go | ma) | i230 | iac(| \ - | \ / ) | ibro | idea | ig01 | ikom | im1k | inno | ipaq | iris | ja(t | v)a | jbro | jemu | jigs | kddi | keji | kgt(| \ / ) | klon | kpt | kwc\ - | kyo(c | k) | le(no | xi) | lg(g | \ / (k | l | u) | 50 | 54 | \ - [a - w]) | libw | lynx | m1\ - w | m3ga | m50\/|ma(te | ui | xo) | mc(01 | 21 | ca) | m\ - cr | me(rc | ri) | mi(o8 | oa | ts) | mmef | mo(01 | 02 | bi | de | do | t(\ - | |o | v) | zz) | mt(50 | p1 | v) | mwbp | mywa | n10[0 - 2] | n20[2 - 3] | n30(0 | 2) | n50(0 | 2 | 5) | n7(0(0 | 1) | 10) | ne((c | m)\ - | on | tf | wf | wg | wt) | nok(6 | i) | nzph | o2im | op(ti | wv) | oran | owg1 | p800 | pan(a | d | t) | pdxg | pg(13 | \ - ([1 - 8] | c)) | phil | pire | pl(ay | uc) | pn\ - 2 | po(ck | rt | se) | prox | psio | pt\ - g | qa\ - a | qc(07 | 12 | 21 | 32 | 60 | \ - [2 - 7] | i\ - ) | qtek | r380 | r600 | raks | rim9 | ro(ve | zo) | s55\/|sa(ge | ma | mm | ms | ny | va) | sc(01 | h\ - | oo | p\ - ) | sdk\/|se(c(\ - | 0 | 1) | 47 | mc | nd | ri) | sgh\ - | shar | sie(\ - | m) | sk\ - 0 | sl(45 | id) | sm(al | ar | b3 | it | t5) | so(ft | ny) | sp(01 | h\ - | v\ - | v) | sy(01 | mb) | t2(18 | 50) | t6(00 | 10 | 18) | ta(gt | lk) | tcl\ - | tdg\ - | tel(i | m) | tim\ - | t\ - mo | to(pl | sh) | ts(70 | m\ - | m3 | m5) | tx\ - 9 | up(\.b | g1 | si) | utst | v400 | v750 | veri | vi(rg | te) | vk(40 | 5[0 - 3] | \ - v) | vm40 | voda | vulc | vx(52 | 53 | 60 | 61 | 70 | 80 | 81 | 83 | 85 | 98) | w3c(\ - | ) | webc | whit | wi(g | nc | nw) | wmlb | wonu | x700 | yas\ - | your | zeto | zte\ - / i.test(a.substr(0, 4))) check = true; })(navigator.userAgent || navigator.vendor || window.opera);
		//return check;
		if (typeof screen.orientation !== 'undefined')
		{
			return false;
		}
		return true;
	}
);


//https://stackoverflow.com/questions/17130395/real-mouse-position-in-canvas
EM_JS
(
	int, WASM_get_CanvasPosX, (),
	{
		//emscripten_border.scrollTop
		//var rect = canvas.getBoundingClientRect();
		//return rect.left;
		return 0;
	}
);

EM_JS
(
	int, WASM_get_CanvasPosY, (),
	{
		//var rect = canvas.getBoundingClientRect();
		//return rect.top;
		//https://stackoverflow.com/questions/11381673/detecting-a-mobile-browser
		if (WASM_IsNotPC())
		{
			var scrollPos = document.getElementsByTagName("html")[0].scrollTop;
			return scrollPos;
		}
		return 0;
	}
);
namespace EMSDK
{
	int		EMSDK_GetBrowserWidth() { return WASM_GetBrowserWidth(); }
	int		EMSDK_GetBrowserHeight() { return WASM_GetBrowserHeight(); }

	int		EMSDK_GetViewportWidth() { return WASM_GetViewportWidth(); }
	int		EMSDK_GetViewportHeight() { return WASM_GetViewportHeight(); }

	bool	EMSDK_IsPC() { return WASM_IsPC(); }
	int		EMSDK_GetCanvasPosX() { return WASM_get_CanvasPosX(); }
	int		EMSDK_GetCanvasPosY() { return WASM_get_CanvasPosY(); }

	void	EMSDK_JSDoFullscreen() { WASM_Fullscreen(); }

	void	EMSDK_JSInit() { WASM_JSInit(); }
	void	EMSDK_JSViewportUpdate() { WASM_JSViewportUpdate(); }

	std::string EMSDK_DodecodeURIComponent(const char* e_str)
	{
		std::string l_str = WASM_DodecodeURIComponent(e_str);
		return l_str;
	}

}

//end #ifdef WASM
#endif
