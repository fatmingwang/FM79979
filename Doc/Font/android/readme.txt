https://stackoverflow.com/questions/8839215/text-to-image-in-android


TextView textView = new TextView(activity.getContext());
textView.setText("Hello World");
textView.setDrawingCacheEnabled(true);
textView.destroyDrawingCache();
textView.buildDrawingCache();
Bitmap bitmap = getTransparentBitmapCopy(textView.getDrawingCache());


private Bitmap getTransparentBitmapCopy(Bitmap source)
{
    int width =  source.getWidth();
    int height = source.getHeight();
    Bitmap copy = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
    int[] pixels = new int[width * height];
    source.getPixels(pixels, 0, width, 0, 0, width, height);
    copy.setPixels(pixels, 0, width, 0, 0, width, height);
    return copy;
}