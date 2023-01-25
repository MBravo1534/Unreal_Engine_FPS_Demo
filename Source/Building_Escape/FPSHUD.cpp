// Developed by Manuel Bravo


#include "FPSHUD.h"

void AFPSHUD::DrawHUD()
{
	Super::DrawHUD();

	if (CrosshairTexture)
	{
		//Find the center of the canvas
		FVector2D Center(Canvas->ClipX * .5f, Canvas->ClipY * .5f);

		// Offset by half of the texture's dimensions so that the center of the texture aligns with the center
		FVector2D CrossHairDrawPosition(Center.X - (CrosshairTexture->GetSurfaceWidth() * .5f), Center.Y - (CrosshairTexture->GetSurfaceHeight() * .5f));

		//Draw the Crosshair at the center point
		FCanvasTileItem TileItem(CrossHairDrawPosition, CrosshairTexture->Resource, FLinearColor::White);
		TileItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(TileItem);
	}
}