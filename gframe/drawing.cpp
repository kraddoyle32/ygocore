#include "game.h"

extern ygo::Game* mainGame;

namespace ygo {

void Game::DrawSelectionLine(irr::video::S3DVertex* vec, bool strip, int width, float* cv) {
	float origin[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	glLineWidth(width);
	glLineStipple(1, linePattern);
	if(strip)
		glEnable(GL_LINE_STIPPLE);
	glDisable(GL_TEXTURE_2D);
	glMaterialfv(GL_FRONT, GL_AMBIENT, cv);
	glBegin(GL_LINE_LOOP);
	glVertex3fv((float*)&vec[0].Pos);
	glVertex3fv((float*)&vec[1].Pos);
	glVertex3fv((float*)&vec[3].Pos);
	glVertex3fv((float*)&vec[2].Pos);
	glEnd();
	glMaterialfv(GL_FRONT, GL_AMBIENT, origin);
	glDisable(GL_LINE_STIPPLE);
}
void Game::DrawBackGround() {
	static int selFieldAlpha = 255;
	static int selFieldDAlpha = -10;
	matrix4 im = irr::core::IdentityMatrix;
	im.setTranslation(vector3df(0, 0, -0.01));
	driver->setTransform(irr::video::ETS_WORLD, im);
	//dark shade
	matManager.mSelField.AmbientColor = 0xff000000;
	matManager.mSelField.DiffuseColor = 0xa0000000;
	driver->setMaterial(matManager.mSelField);
	for(int i = 0; i < 120; i += 4)
		driver->drawVertexPrimitiveList(&matManager.vFields[i], 4, matManager.iRectangle, 2);
	driver->setTransform(irr::video::ETS_WORLD, irr::core::IdentityMatrix);
	driver->setMaterial(matManager.mBackLine);
	driver->drawVertexPrimitiveList(matManager.vBackLine, 76, matManager.iBackLine, 58, irr::video::EVT_STANDARD, irr::scene::EPT_LINES);
	//select field
	if(dInfo.curMsg == MSG_SELECT_PLACE || dInfo.curMsg == MSG_SELECT_DISFIELD) {
		float cv[4] = {0.0f, 0.0f, 1.0f, 1.0f};
		int filter = 0x1;
		for (int i = 0; i < 5; ++i, filter <<= 1) {
			if ((dField.selectable_field & filter) > 0)
				DrawSelectionLine(&matManager.vFields[16 + i * 4], !(dField.selected_field & filter), 2, cv);
		}
		filter = 0x100;
		for (int i = 0; i < 5; ++i, filter <<= 1) {
			if ((dField.selectable_field & filter) > 0)
				DrawSelectionLine(&matManager.vFields[36 + i * 4], !(dField.selected_field & filter), 2, cv);
		}
		filter = 0x10000;
		for (int i = 0; i < 5; ++i, filter <<= 1) {
			if ((dField.selectable_field & filter) > 0)
				DrawSelectionLine(&matManager.vFields[76 + i * 4], !(dField.selected_field & filter), 2, cv);
		}
		filter = 0x1000000;
		for (int i = 0; i < 5; ++i, filter <<= 1) {
			if ((dField.selectable_field & filter) > 0)
				DrawSelectionLine(&matManager.vFields[96 + i * 4], !(dField.selected_field & filter), 2, cv);
		}
	}
	//disabled field
	{
		float cv[4] = {0.0f, 0.0f, 1.0f, 1.0f};
		int filter = 0x1;
		for (int i = 0; i < 5; ++i, filter <<= 1) {
			if ((dField.disabled_field & filter) > 0) {
				driver->draw3DLine(matManager.vFields[16 + i * 4].Pos, matManager.vFields[16 + i * 4 + 3].Pos, 0xffffffff);
				driver->draw3DLine(matManager.vFields[16 + i * 4 + 1].Pos, matManager.vFields[16 + i * 4 + 2].Pos, 0xffffffff);
			}
		}
		filter = 0x100;
		for (int i = 0; i < 5; ++i, filter <<= 1) {
			if ((dField.disabled_field & filter) > 0) {
				driver->draw3DLine(matManager.vFields[36 + i * 4].Pos, matManager.vFields[36 + i * 4 + 3].Pos, 0xffffffff);
				driver->draw3DLine(matManager.vFields[36 + i * 4 + 1].Pos, matManager.vFields[36 + i * 4 + 2].Pos, 0xffffffff);
			}
		}
		filter = 0x10000;
		for (int i = 0; i < 5; ++i, filter <<= 1) {
			if ((dField.disabled_field & filter) > 0) {
				driver->draw3DLine(matManager.vFields[76 + i * 4].Pos, matManager.vFields[76 + i * 4 + 3].Pos, 0xffffffff);
				driver->draw3DLine(matManager.vFields[76 + i * 4 + 1].Pos, matManager.vFields[76 + i * 4 + 2].Pos, 0xffffffff);
			}
		}
		filter = 0x1000000;
		for (int i = 0; i < 5; ++i, filter <<= 1) {
			if ((dField.disabled_field & filter) > 0) {
				driver->draw3DLine(matManager.vFields[96 + i * 4].Pos, matManager.vFields[96 + i * 4 + 3].Pos, 0xffffffff);
				driver->draw3DLine(matManager.vFields[96 + i * 4 + 1].Pos, matManager.vFields[96 + i * 4 + 2].Pos, 0xffffffff);
			}
		}
	}
	//current sel
	if (dField.hovered_location != 0 && dField.hovered_location  != 2) {
		int index = 0;
		if (dField.hovered_controler == 0) {
			if (dField.hovered_location == LOCATION_DECK) index = 0;
			else if (dField.hovered_location == LOCATION_MZONE) index = 16 + dField.hovered_sequence * 4;
			else if (dField.hovered_location == LOCATION_SZONE) index = 36 + dField.hovered_sequence * 4;
			else if (dField.hovered_location == LOCATION_GRAVE) index = 4;
			else if (dField.hovered_location == LOCATION_REMOVED) index = 12;
			else if (dField.hovered_location == LOCATION_EXTRA) index = 8;
		} else {
			if (dField.hovered_location == LOCATION_DECK) index = 60;
			else if (dField.hovered_location == LOCATION_MZONE) index = 76 + dField.hovered_sequence * 4;
			else if (dField.hovered_location == LOCATION_SZONE) index = 96 + dField.hovered_sequence * 4;
			else if (dField.hovered_location == LOCATION_GRAVE) index = 64;
			else if (dField.hovered_location == LOCATION_REMOVED) index = 72;
			else if (dField.hovered_location == LOCATION_EXTRA) index = 68;
		}
		selFieldAlpha += selFieldDAlpha;
		if (selFieldAlpha <= 5) {
			selFieldAlpha = 5;
			selFieldDAlpha = 10;
		}
		if (selFieldAlpha >= 205) {
			selFieldAlpha = 205;
			selFieldDAlpha = -10;
		}
		matManager.mSelField.AmbientColor = 0xffffffff;
		matManager.mSelField.DiffuseColor = selFieldAlpha << 24;
		driver->setMaterial(matManager.mSelField);
		driver->drawVertexPrimitiveList(&matManager.vFields[index], 4, matManager.iRectangle, 2);
	}
}
void Game::DrawCards() {
	for(int p = 0; p < 2; ++p) {
		for(int i = 0; i < 5; ++i)
			if(dField.mzone[p][i])
				DrawCard(dField.mzone[p][i]);
		for(int i = 0; i < 6; ++i)
			if(dField.szone[p][i])
				DrawCard(dField.szone[p][i]);
		for(int i = 0; i < dField.deck[p].size(); ++i)
			DrawCard(dField.deck[p][i]);
		for(int i = 0; i < dField.hand[p].size(); ++i)
			DrawCard(dField.hand[p][i]);
		for(int i = 0; i < dField.grave[p].size(); ++i)
			DrawCard(dField.grave[p][i]);
		for(int i = 0; i < dField.remove[p].size(); ++i)
			DrawCard(dField.remove[p][i]);
		for(int i = 0; i < dField.extra[p].size(); ++i)
			DrawCard(dField.extra[p][i]);
	}
	std::set<ClientCard*>::iterator cit;
	for(cit = dField.overlay_cards.begin(); cit != dField.overlay_cards.end(); ++cit)
		DrawCard(*cit);
}
void Game::DrawCard(ClientCard* pcard) {
	driver->setTransform(irr::video::ETS_WORLD, pcard->mTransform);
	if(pcard->aniFrame) {
		if(pcard->is_moving) {
			pcard->curPos += pcard->dPos;
			pcard->curRot += pcard->dRot;
			pcard->mTransform.setTranslation(pcard->curPos);
			pcard->mTransform.setRotationRadians(pcard->curRot);
		}
		if(pcard->is_fading)
			pcard->curAlpha += pcard->dAlpha;
		pcard->aniFrame--;
		if(pcard->aniFrame == 0) {
			pcard->is_moving = false;
			pcard->is_fading = false;
		}
	}
	matManager.mCard.AmbientColor = 0xffffffff;
	matManager.mCard.DiffuseColor = (pcard->curAlpha << 24) | 0xffffff;
	matManager.mCard.setTexture(0, imageManager.GetTexture(pcard->code));
	driver->setTransform(irr::video::ETS_WORLD, pcard->mTransform);
	driver->setMaterial(matManager.mCard);
	driver->drawVertexPrimitiveList(matManager.vCardFront, 4, matManager.iRectangle, 2);
	matManager.mCard.setTexture(0, imageManager.tCover);
	driver->setMaterial(matManager.mCard);
	driver->drawVertexPrimitiveList(matManager.vCardBack, 4, matManager.iRectangle, 2);
	if(pcard->is_showequip) {
		matManager.mTexture.setTexture(0, imageManager.tEquip);
		driver->setMaterial(matManager.mTexture);
		driver->drawVertexPrimitiveList(matManager.vSymbol, 4, matManager.iRectangle, 2);
	} else if(pcard->is_showtarget) {
		matManager.mTexture.setTexture(0, imageManager.tTarget);
		driver->setMaterial(matManager.mTexture);
		driver->drawVertexPrimitiveList(matManager.vSymbol, 4, matManager.iRectangle, 2);
	} else if(pcard->is_disabled && (pcard->location & LOCATION_ONFIELD) && (pcard->position & POS_FACEUP)) {
		matManager.mTexture.setTexture(0, imageManager.tNegated);
		driver->setMaterial(matManager.mTexture);
		driver->drawVertexPrimitiveList(matManager.vNegate, 4, matManager.iRectangle, 2);
	}
	if(pcard->is_selectable && (pcard->location & 0xe)) {
		float cv[4] = {1.0f, 1.0f, 0.0f, 1.0f};
		if((pcard->location == LOCATION_HAND && pcard->code) || ((pcard->location & 0xc) && (pcard->position & POS_FACEUP)))
			DrawSelectionLine(matManager.vCardOutline, !pcard->is_selected, 2, cv);
		else
			DrawSelectionLine(matManager.vCardOutliner, !pcard->is_selected, 2, cv);
	}
	if(pcard->is_highlighting) {
		float cv[4] = {0.0f, 1.0f, 1.0f, 1.0f};
		if((pcard->location == LOCATION_HAND && pcard->code) || ((pcard->location & 0xc) && (pcard->position & POS_FACEUP)))
			DrawSelectionLine(matManager.vCardOutline, true, 2, cv);
		else
			DrawSelectionLine(matManager.vCardOutliner, true, 2, cv);
	}
	if(pcard->cmdFlag & COMMAND_ATTACK) {
		matManager.mTexture.setTexture(0, imageManager.tAttack);
		driver->setMaterial(matManager.mTexture);
		irr::core::matrix4 atk;
		atk.setTranslation(pcard->curPos + vector3df(0, -atkdy / 4.0f - 0.35f, 0.05f));
		driver->setTransform(irr::video::ETS_WORLD, atk);
		driver->drawVertexPrimitiveList(matManager.vSymbol, 4, matManager.iRectangle, 2);
	}
}
void Game::DrawMisc() {
	static irr::core::vector3df act_rot(0, 0, 0);
	irr::core::matrix4 im, ic, it;
	act_rot.Z += 0.02;
	im.setRotationRadians(act_rot);
	matManager.mTexture.setTexture(0, imageManager.tAct);
	driver->setMaterial(matManager.mTexture);
	if(dField.deck_act) {
		im.setTranslation(vector3df(7.3f, 3.0f, dField.deck[0].size() * 0.01f + 0.02f));
		driver->setTransform(irr::video::ETS_WORLD, im);
		driver->drawVertexPrimitiveList(matManager.vActivate, 4, matManager.iRectangle, 2);
	}
	if(dField.grave_act) {
		im.setTranslation(vector3df(7.3f, 1.7f, dField.grave[0].size() * 0.01f + 0.02f));
		driver->setTransform(irr::video::ETS_WORLD, im);
		driver->drawVertexPrimitiveList(matManager.vActivate, 4, matManager.iRectangle, 2);
	}
	if(dField.remove_act) {
		im.setTranslation(vector3df(7.3f, 0.4f, dField.remove[0].size() * 0.01f + 0.02f));
		driver->setTransform(irr::video::ETS_WORLD, im);
		driver->drawVertexPrimitiveList(matManager.vActivate, 4, matManager.iRectangle, 2);
	}
	if(dField.extra_act) {
		im.setTranslation(vector3df(0.6f, 3.0f, dField.extra[0].size() * 0.01f + 0.02f));
		driver->setTransform(irr::video::ETS_WORLD, im);
		driver->drawVertexPrimitiveList(matManager.vActivate, 4, matManager.iRectangle, 2);
	}
	if(dField.chains.size() > 1) {
		for(int i = 0; i < dField.chains.size(); ++i) {
			if(dField.chains[i].solved)
				break;
			matManager.mTRTexture.setTexture(0, imageManager.tChain);
			matManager.mTRTexture.AmbientColor = 0xffffff00;
			ic.setRotationRadians(act_rot);
			ic.setTranslation(dField.chains[i].chain_pos);
			driver->setMaterial(matManager.mTRTexture);
			driver->setTransform(irr::video::ETS_WORLD, ic);
			driver->drawVertexPrimitiveList(matManager.vSymbol, 4, matManager.iRectangle, 2);
			it.setScale(0.6f);
			it.setTranslation(dField.chains[i].chain_pos);
			matManager.mTRTexture.setTexture(0, imageManager.tNumber);
			matManager.vChainNum[0].TCoords = vector2df(0.19375f * (i % 5), 0.2421875f * (i / 5));
			matManager.vChainNum[1].TCoords = vector2df(0.19375f * (i % 5 + 1), 0.2421875f * (i / 5));
			matManager.vChainNum[2].TCoords = vector2df(0.19375f * (i % 5), 0.2421875f * (i / 5 + 1));
			matManager.vChainNum[3].TCoords = vector2df(0.19375f * (i % 5 + 1), 0.2421875f * (i / 5 + 1));
			driver->setMaterial(matManager.mTRTexture);
			driver->setTransform(irr::video::ETS_WORLD, it);
			driver->drawVertexPrimitiveList(matManager.vChainNum, 4, matManager.iRectangle, 2);
		}
	}
	//lp bar
	if((dInfo.turn % 2 && dInfo.is_first_turn) || (!(dInfo.turn % 2) && !dInfo.is_first_turn)) {
		driver->draw2DRectangle(0xa0000000, recti(327, 8, 630, 51));
		driver->draw2DRectangleOutline(recti(327, 8, 630, 51), 0xffff8080);
	} else {
		driver->draw2DRectangle(0xa0000000, recti(689, 8, 991, 51));
		driver->draw2DRectangleOutline(recti(689, 8, 991, 51), 0xffff8080);
	}
	driver->draw2DImage(imageManager.tLPFrame, recti(330, 10, 629, 30), recti(0, 0, 200, 20), 0, 0, true);
	driver->draw2DImage(imageManager.tLPFrame, recti(691, 10, 990, 30), recti(0, 0, 200, 20), 0, 0, true);
	if(dInfo.lp[0] >= 8000)
		driver->draw2DImage(imageManager.tLPBar, recti(335, 12, 625, 28), recti(0, 0, 16, 16), 0, 0, true);
	else driver->draw2DImage(imageManager.tLPBar, recti(335, 12, 335 + 290 * dInfo.lp[0] / 8000, 28), recti(0, 0, 16, 16), 0, 0, true);
	if(dInfo.lp[1] >= 8000)
		driver->draw2DImage(imageManager.tLPBar, recti(696, 12, 986, 28), recti(0, 0, 16, 16), 0, 0, true);
	else driver->draw2DImage(imageManager.tLPBar, recti(986 - 290 * dInfo.lp[1] / 8000, 12, 986 , 28), recti(0, 0, 16, 16), 0, 0, true);
	if(lpframe) {
		dInfo.lp[lpplayer] -= lpd;
		myswprintf(dInfo.strLP[lpplayer], L"%d", dInfo.lp[lpplayer]);
		lpccolor -= 0x19000000;
		lpframe--;
	}
	if(lpcstring) {
		if(lpplayer == 0) {
			lpcFont->draw(lpcstring, recti(400, 470, 920, 520), lpccolor | 0x00ffffff, true, false, 0);
			lpcFont->draw(lpcstring, recti(400, 472, 922, 520), lpccolor, true, false, 0);
		} else {
			lpcFont->draw(lpcstring, recti(400, 160, 920, 210), lpccolor | 0x00ffffff, true, false, 0);
			lpcFont->draw(lpcstring, recti(400, 162, 922, 210), lpccolor, true, false, 0);
		}
	}
	numFont->draw(dInfo.strLP[0], recti(330, 11, 629, 30), 0xff000000, true, false, 0);
	numFont->draw(dInfo.strLP[0], recti(330, 12, 631, 30), 0xffffff00, true, false, 0);
	numFont->draw(dInfo.strLP[1], recti(691, 11, 990, 30), 0xff000000, true, false, 0);
	numFont->draw(dInfo.strLP[1], recti(691, 12, 992, 30), 0xffffff00, true, false, 0);

	textFont->draw(dInfo.hostname, recti(335, 31, 629, 50), 0xffffffff, false, false, 0);
	auto cld = textFont->getDimension(dInfo.clientname);
	textFont->draw(dInfo.clientname, recti(986 - cld.Width, 31, 986, 50), 0xffffffff, false, false, 0);

	driver->draw2DRectangle(recti(632, 10, 688, 30), 0x00000000, 0x00000000, 0xffffffff, 0xffffffff);
	driver->draw2DRectangle(recti(632, 30, 688, 50), 0xffffffff, 0xffffffff, 0x00000000, 0x00000000);
	lpcFont->draw(dataManager.GetNumString(dInfo.turn), recti(635, 5, 685, 40), 0x80000000, true, false, 0);
	lpcFont->draw(dataManager.GetNumString(dInfo.turn), recti(635, 5, 687, 40), 0x8000ffff, true, false, 0);
	for(int i = 0; i < 5; ++i) {
		ClientCard* pcard = dField.mzone[0][i];
		if(pcard) {
			int m = 491 + i * 86;
			adFont->draw(L"/", recti(m - 4, 403, m + 4, 423), 0xff000000, true, false, 0);
			adFont->draw(L"/", recti(m - 3, 404, m + 5, 424), 0xffffffff, true, false, 0);
			int w = adFont->getDimension(pcard->atkstring).Width;
			adFont->draw(pcard->atkstring, recti(m - 5 - w, 403, m - 5, 423), 0xff000000, false, false, 0);
			adFont->draw(pcard->atkstring, recti(m - 4 - w, 404, m - 4, 424),
			             pcard->attack > pcard->base_attack ? 0xffffff00 : pcard->attack < pcard->base_attack ? 0xffff2090 : 0xffffffff , false, false, 0);
			w = adFont->getDimension(pcard->defstring).Width;
			adFont->draw(pcard->defstring, recti(m + 4, 403, m + 4 + w, 423), 0xff000000, false, false, 0);
			adFont->draw(pcard->defstring, recti(m + 5, 404, m + 5 + w, 424),
			             pcard->defence > pcard->base_defence ? 0xffffff00 : pcard->defence < pcard->base_defence ? 0xffff2090 : 0xffffffff , false, false, 0);
			adFont->draw(pcard->lvstring, recti(468 + i * 83, 347, 500 + i * 83, 367), 0xff000000, false, false, 0);
			adFont->draw(pcard->lvstring, recti(469 + i * 83, 348, 501 + i * 83, 368),
			             (pcard->type & TYPE_XYZ) ? 0xffff80ff : (pcard->type & TYPE_TUNER) ? 0xffffff00 : 0xffffffff, false, false, 0);
		}
	}
	for(int i = 0; i < 5; ++i) {
		ClientCard* pcard = dField.mzone[1][i];
		if(pcard && (pcard->position & POS_FACEUP)) {
			int m = 808 - i * 72;
			adFont->draw(L"/", recti(m - 4, 235, m + 4, 255), 0xff000000, true, false, 0);
			adFont->draw(L"/", recti(m - 3, 236, m + 5, 256), 0xffffffff, true, false, 0);
			int w = adFont->getDimension(pcard->atkstring).Width;
			adFont->draw(pcard->atkstring, recti(m - 5 - w, 235, m - 5, 255), 0xff000000, false, false, 0);
			adFont->draw(pcard->atkstring, recti(m - 4 - w, 236, m - 4, 256),
			             pcard->attack > pcard->base_attack ? 0xffffff00 : pcard->attack < pcard->base_attack ? 0xffff2090 : 0xffffffff , false, false, 0);
			w = adFont->getDimension(pcard->defstring).Width;
			adFont->draw(pcard->defstring, recti(m + 4, 235, m + 4 + w, 255), 0xff000000, false, false, 0);
			adFont->draw(pcard->defstring, recti(m + 5, 236, m + 5 + w, 256),
			             pcard->defence > pcard->base_defence ? 0xffffff00 : pcard->defence < pcard->base_defence ? 0xffff2090 : 0xffffffff , false, false, 0);
			adFont->draw(pcard->lvstring, recti(789 - i * 76, 280, 820 - i * 76, 300), 0xff000000, false, false, 0);
			adFont->draw(pcard->lvstring, recti(789 - i * 76, 281, 820 - i * 76, 301),
			             (pcard->type & TYPE_XYZ) ? 0xffff80ff : (pcard->type & TYPE_TUNER) ? 0xffffff00 : 0xffffffff, false, false, 0);
		}
	}
	if(dField.extra[0].size()) {
		numFont->draw(dataManager.GetNumString(dField.extra[0].size()), recti(337, 540, 397, 560), 0xff000000, true, false, 0);
		numFont->draw(dataManager.GetNumString(dField.extra[0].size()), recti(337, 541, 399, 561), 0xffffff00, true, false, 0);
	}
	if(dField.deck[0].size()) {
		numFont->draw(dataManager.GetNumString(dField.deck[0].size()), recti(934, 540, 994, 560), 0xff000000, true, false, 0);
		numFont->draw(dataManager.GetNumString(dField.deck[0].size()), recti(934, 541, 996, 561), 0xffffff00, true, false, 0);
	}
	if(dField.grave[0].size()) {
		numFont->draw(dataManager.GetNumString(dField.grave[0].size()), recti(910, 442, 970, 462), 0xff000000, true, false, 0);
		numFont->draw(dataManager.GetNumString(dField.grave[0].size()), recti(910, 443, 972, 463), 0xffffff00, true, false, 0);
	}
	if(dField.remove[0].size()) {
		numFont->draw(dataManager.GetNumString(dField.remove[0].size()), recti(888, 359, 948, 359), 0xff000000, true, false, 0);
		numFont->draw(dataManager.GetNumString(dField.remove[0].size()), recti(888, 360, 950, 360), 0xffffff00, true, false, 0);
	}
	if(dField.extra[1].size()) {
		numFont->draw(dataManager.GetNumString(dField.extra[1].size()), recti(848, 194, 908, 214), 0xff000000, true, false, 0);
		numFont->draw(dataManager.GetNumString(dField.extra[1].size()), recti(848, 195, 910, 215), 0xffffff00, true, false, 0);
	}
	if(dField.deck[1].size()) {
		numFont->draw(dataManager.GetNumString(dField.deck[1].size()), recti(421, 194, 481, 214), 0xff000000, true, false, 0);
		numFont->draw(dataManager.GetNumString(dField.deck[1].size()), recti(421, 195, 483, 215), 0xffffff00, true, false, 0);
	}
	if(dField.grave[1].size()) {
		numFont->draw(dataManager.GetNumString(dField.grave[1].size()), recti(408, 250, 469, 270), 0xff000000, true, false, 0);
		numFont->draw(dataManager.GetNumString(dField.grave[1].size()), recti(408, 251, 471, 271), 0xffffff00, true, false, 0);
	}
	if(dField.remove[1].size()) {
		numFont->draw(dataManager.GetNumString(dField.remove[1].size()), recti(392, 315, 453, 335), 0xff000000, true, false, 0);
		numFont->draw(dataManager.GetNumString(dField.remove[1].size()), recti(392, 316, 455, 336), 0xffffff00, true, false, 0);
	}
}
void Game::DrawGUI() {
	if(imageLoading.size()) {
		std::map<irr::gui::CGUIImageButton*, int>::iterator mit;
		for(mit = imageLoading.begin(); mit != imageLoading.end(); ++mit)
			mit->first->setImage(imageManager.GetTexture(mit->second));
		imageLoading.clear();
	}
	if(fadingFrame) {
		if(isFadein) {
			if(fadingFrame > 5) {
				fadingUL.X -= fadingDiff.X;
				fadingLR.X += fadingDiff.X;
				fadingFrame--;
				guiFading->setRelativePosition(irr::core::recti(fadingUL, fadingLR));
			} else {
				fadingUL.Y -= fadingDiff.Y;
				fadingLR.Y += fadingDiff.Y;
				fadingFrame--;
				if(!fadingFrame) {
					guiFading->setRelativePosition(fadingSize);
					if(guiFading == wPosSelect) {
						btnPSAU->setDrawImage(true);
						btnPSAD->setDrawImage(true);
						btnPSDU->setDrawImage(true);
						btnPSDD->setDrawImage(true);
					}
					if(guiFading == wCardSelect) {
						for(int i = 0; i < 5; ++i)
							btnCardSelect[i]->setDrawImage(true);
					}
				} else
					guiFading->setRelativePosition(irr::core::recti(fadingUL, fadingLR));
			}
		} else {
			if(fadingFrame > 5) {
				fadingUL.Y += fadingDiff.Y;
				fadingLR.Y -= fadingDiff.Y;
				fadingFrame--;
				guiFading->setRelativePosition(irr::core::recti(fadingUL, fadingLR));
			} else {
				fadingUL.X += fadingDiff.X;
				fadingLR.X -= fadingDiff.X;
				fadingFrame--;
				if(!fadingFrame) {
					guiFading->setVisible(false);
					guiFading->setRelativePosition(fadingSize);
					if(guiFading == wPosSelect) {
						btnPSAU->setDrawImage(true);
						btnPSAD->setDrawImage(true);
						btnPSDU->setDrawImage(true);
						btnPSDD->setDrawImage(true);
					}
					if(guiFading == wCardSelect) {
						for(int i = 0; i < 5; ++i)
							btnCardSelect[i]->setDrawImage(true);
					}
				} else
					guiFading->setRelativePosition(irr::core::recti(fadingUL, fadingLR));
			}
		}
		if(signalAction && !fadingFrame) {
			localAction.Set();
			signalAction = false;
		}
	} else if(autoFadeoutFrame) {
		autoFadeoutFrame--;
		if(!autoFadeoutFrame)
			HideElement(guiFading);
	}
	env->drawAll();
}
void Game::DrawSpec() {
	if(showcard) {
		switch(showcard) {
		case 1: {
			driver->draw2DImage(imageManager.GetTexture(showcardcode), position2di(574, 200));
			driver->draw2DImage(imageManager.tMask, recti(574, 200, 574 + (showcarddif > 177 ? 177 : showcarddif), 454),
			                    recti(254 - showcarddif, 0, 254 - (showcarddif > 177 ? showcarddif - 177 : 0), 254), 0, 0, true);
			showcarddif += 15;
			if(showcarddif >= 254) {
				showcard = 2;
				showcarddif = 0;
			}
			break;
		}
		case 2: {
			driver->draw2DImage(imageManager.GetTexture(showcardcode), position2di(574, 200));
			driver->draw2DImage(imageManager.tMask, recti(574 + showcarddif, 200, 761, 454), recti(0, 0, 177 - showcarddif, 254), 0, 0, true);
			showcarddif += 15;
			if(showcarddif >= 177) {
				showcard = 0;
			}
			break;
		}
		case 3: {
			driver->draw2DImage(imageManager.GetTexture(showcardcode), position2di(574, 200));
			driver->draw2DImage(imageManager.tNegated, recti(536 + showcarddif, 191 + showcarddif, 793 - showcarddif, 447 - showcarddif), recti(0, 0, 128, 128), 0, 0, true);
			if(showcarddif < 64)
				showcarddif += 4;
			break;
		}
		case 4: {
			matManager.c2d[0] = (showcarddif << 24) | 0xffffff;
			matManager.c2d[1] = (showcarddif << 24) | 0xffffff;
			matManager.c2d[2] = (showcarddif << 24) | 0xffffff;
			matManager.c2d[3] = (showcarddif << 24) | 0xffffff;
			driver->draw2DImage(imageManager.GetTexture(showcardcode), recti(574, 200, 751, 454),
			                    recti(0, 0, 177, 254), 0, matManager.c2d, true);
			if(showcarddif < 255)
				showcarddif += 17;
			break;
		}
		case 5: {
			matManager.c2d[0] = (showcarddif << 25) | 0xffffff;
			matManager.c2d[1] = (showcarddif << 25) | 0xffffff;
			matManager.c2d[2] = (showcarddif << 25) | 0xffffff;
			matManager.c2d[3] = (showcarddif << 25) | 0xffffff;
			driver->draw2DImage(imageManager.GetTexture(showcardcode), recti(662 - showcarddif * 0.69685f, 327 - showcarddif, 662 + showcarddif * 0.69685f, 327 + showcarddif),
			                    recti(0, 0, 177, 254), 0, matManager.c2d, true);
			if(showcarddif < 127)
				showcarddif += 9;
			break;
		}
		case 6: {
			driver->draw2DImage(imageManager.GetTexture(showcardcode), position2di(574, 200));
			driver->draw2DImage(imageManager.tNumber, recti(536 + showcarddif, 191 + showcarddif, 793 - showcarddif, 447 - showcarddif),
			                    recti((showcardp % 5) * 32, (showcardp / 5) * 32, (showcardp % 5 + 1) * 32, (showcardp / 5 + 1) * 32), 0, 0, true);
			if(showcarddif < 64)
				showcarddif += 4;
			break;
		}
		}
	}
	if(is_attacking) {
		irr::core::matrix4 matk;
		matk.setTranslation(atk_t);
		matk.setRotationRadians(atk_r);
		driver->setTransform(irr::video::ETS_WORLD, matk);
		driver->setMaterial(matManager.mATK);
		driver->drawVertexPrimitiveList(&matManager.vArrow[attack_sv], 40, matManager.iArrow, 10, EVT_STANDARD, EPT_TRIANGLE_STRIP);
		attack_sv += 4;
		if (attack_sv > 28)
			attack_sv = 0;
	}
}
void Game::ShowElement(irr::gui::IGUIElement * win, int autoframe) {
	if(fadingFrame || win->isVisible())
		return;
	fadingSize = win->getRelativePosition();
	irr::core::position2di center = fadingSize.getCenter();
	fadingDiff.X = fadingSize.getWidth() / 10;
	fadingDiff.Y = (fadingSize.getHeight() - 4) / 10;
	fadingUL = center;
	fadingLR = center;
	fadingUL.Y -= 2;
	fadingLR.Y += 2;
	guiFading = win;
	isFadein = true;
	fadingFrame = 10;
	autoFadeoutFrame = autoframe;
	if(win == wPosSelect) {
		btnPSAU->setDrawImage(false);
		btnPSAD->setDrawImage(false);
		btnPSDU->setDrawImage(false);
		btnPSDD->setDrawImage(false);
	}
	if(win == wCardSelect) {
		for(int i = 0; i < 5; ++i)
			btnCardSelect[i]->setDrawImage(false);
	}
	win->setRelativePosition(irr::core::recti(center.X, center.Y, 0, 0));
	win->setVisible(true);
}
void Game::HideElement(irr::gui::IGUIElement * win, bool set_action) {
	if(fadingFrame || !win->isVisible())
		return;
	fadingSize = win->getRelativePosition();
	fadingDiff.X = fadingSize.getWidth() / 10;
	fadingDiff.Y = (fadingSize.getHeight() - 4) / 10;
	fadingUL = fadingSize.UpperLeftCorner;
	fadingLR = fadingSize.LowerRightCorner;
	guiFading = win;
	isFadein = false;
	fadingFrame = 10;
	autoFadeoutFrame = 0;
	signalAction = set_action;
	if(win == wPosSelect) {
		btnPSAU->setDrawImage(false);
		btnPSAD->setDrawImage(false);
		btnPSDU->setDrawImage(false);
		btnPSDD->setDrawImage(false);
	}
	if(win == wCardSelect) {
		for(int i = 0; i < 5; ++i)
			btnCardSelect[i]->setDrawImage(false);
	}
}
void Game::PopupElement(irr::gui::IGUIElement * element, int hideframe) {
	element->getParent()->bringToFront(element);
	dField.panel = element;
	env->setFocus(element);
	if(!hideframe)
		ShowElement(element);
	else ShowElement(element, hideframe);
}
void Game::WaitFrameSignal(int frame) {
	frameSignal.Reset();
	signalFrame = frame;
	frameSignal.Wait();
}
void Game::DrawThumb(int code, position2di pos, std::unordered_map<int, int>* lflist) {
	driver->draw2DImage(imageManager.GetTextureThumb(code), pos);
	if(lflist->count(code)) {
		switch((*lflist)[code]) {
		case 0:
			driver->draw2DImage(imageManager.tLim, recti(pos.X, pos.Y, pos.X + 20, pos.Y + 20), recti(0, 0, 64, 64), 0, 0, true);
			break;
		case 1:
			driver->draw2DImage(imageManager.tLim, recti(pos.X, pos.Y, pos.X + 20, pos.Y + 20), recti(64, 0, 128, 64), 0, 0, true);
			break;
		case 2:
			driver->draw2DImage(imageManager.tLim, recti(pos.X, pos.Y, pos.X + 20, pos.Y + 20), recti(0, 64, 64, 128), 0, 0, true);
			break;
		}
	}
}
void Game::DrawDeckBd() {
	wchar_t textBuffer[32];
	//main deck
	driver->draw2DRectangle(recti(310, 137, 410, 157), 0x400000ff, 0x400000ff, 0x40000000, 0x40000000);
	driver->draw2DRectangleOutline(recti(309, 136, 410, 157));
	textFont->draw(L"主卡组：", recti(314, 136, 409, 156), 0xff000000, false, true);
	textFont->draw(L"主卡组：", recti(315, 137, 410, 157), 0xffffffff, false, true);
	numFont->draw(dataManager.numStrings[deckManager.deckhost.main.size()], recti(379, 137, 439, 157), 0xff000000, false, true);
	numFont->draw(dataManager.numStrings[deckManager.deckhost.main.size()], recti(380, 138, 440, 158), 0xffffffff, false, true);
	driver->draw2DRectangle(recti(310, 160, 797, 436), 0x400000ff, 0x400000ff, 0x40000000, 0x40000000);
	driver->draw2DRectangleOutline(recti(309, 159, 797, 436));
	int lx;
	float dx;
	if(deckManager.deckhost.main.size() <= 40) {
		dx = 436.0f / 9;
		lx = 10;
	} else {
		lx = (deckManager.deckhost.main.size() - 41) / 4 + 11;
		dx = 436.0f / (lx - 1);
	}
	for(int i = 0; i < deckManager.deckhost.main.size(); ++i) {
		DrawThumb(deckManager.deckhost.main[i]->first, position2di(314 + (i % lx) * dx, 164 + (i / lx) * 68), deckBuilder.filterList);
		if(deckBuilder.hovered_pos == 1 && deckBuilder.hovered_seq == i)
			driver->draw2DRectangleOutline(recti(313 + (i % lx) * dx, 163 + (i / lx) * 68, 359 + (i % lx) * dx, 228 + (i / lx) * 68));
	}
	//extra deck
	driver->draw2DRectangle(recti(310, 440, 410, 460), 0x400000ff, 0x400000ff, 0x40000000, 0x40000000);
	driver->draw2DRectangleOutline(recti(309, 439, 410, 460));
	textFont->draw(L"额外卡组：", recti(314, 439, 409, 459), 0xff000000, false, true);
	textFont->draw(L"额外卡组：", recti(315, 440, 410, 460), 0xffffffff, false, true);
	numFont->draw(dataManager.numStrings[deckManager.deckhost.extra.size()], recti(379, 440, 439, 460), 0xff000000, false, true);
	numFont->draw(dataManager.numStrings[deckManager.deckhost.extra.size()], recti(380, 441, 440, 461), 0xffffffff, false, true);
	driver->draw2DRectangle(recti(310, 463, 797, 533), 0x400000ff, 0x400000ff, 0x40000000, 0x40000000);
	driver->draw2DRectangleOutline(recti(309, 462, 797, 533));
	if(deckManager.deckhost.extra.size() <= 10)
		dx = 436.0f / 9;
	else dx = 436.0f / (deckManager.deckhost.extra.size() - 1);
	for(int i = 0; i < deckManager.deckhost.extra.size(); ++i) {
		DrawThumb(deckManager.deckhost.extra[i]->first, position2di(314 + i * dx, 466), deckBuilder.filterList);
		if(deckBuilder.hovered_pos == 2 && deckBuilder.hovered_seq == i)
			driver->draw2DRectangleOutline(recti(313 + i * dx, 465, 359 + i * dx, 531));
	}
	//side deck
	driver->draw2DRectangle(recti(310, 537, 410, 557), 0x400000ff, 0x400000ff, 0x40000000, 0x40000000);
	driver->draw2DRectangleOutline(recti(309, 536, 410, 557));
	textFont->draw(L"副卡组：", recti(314, 536, 409, 556), 0xff000000, false, true);
	textFont->draw(L"副卡组：", recti(315, 537, 410, 557), 0xffffffff, false, true);
	numFont->draw(dataManager.numStrings[deckManager.deckhost.side.size()], recti(379, 537, 439, 557), 0xff000000, false, true);
	numFont->draw(dataManager.numStrings[deckManager.deckhost.side.size()], recti(380, 538, 440, 558), 0xffffffff, false, true);
	driver->draw2DRectangle(recti(310, 560, 797, 630), 0x400000ff, 0x400000ff, 0x40000000, 0x40000000);
	driver->draw2DRectangleOutline(recti(309, 559, 797, 630));
	if(deckManager.deckhost.side.size() <= 10)
		dx = 436.0f / 9;
	else dx = 436.0f / (deckManager.deckhost.side.size() - 1);
	for(int i = 0; i < deckManager.deckhost.side.size(); ++i) {
		DrawThumb(deckManager.deckhost.side[i]->first, position2di(314 + i * dx, 564), deckBuilder.filterList);
		if(deckBuilder.hovered_pos == 3 && deckBuilder.hovered_seq == i)
			driver->draw2DRectangleOutline(recti(313 + i * dx, 563, 359 + i * dx, 629));
	}
	driver->draw2DRectangle(recti(805, 137, 915, 157), 0x400000ff, 0x400000ff, 0x40000000, 0x40000000);
	driver->draw2DRectangleOutline(recti(804, 136, 915, 157));
	textFont->draw(L"查找结果：", recti(809, 136, 914, 156), 0xff000000, false, true);
	textFont->draw(L"查找结果：", recti(810, 137, 915, 157), 0xffffffff, false, true);
	numFont->draw(deckBuilder.result_string, recti(874, 136, 934, 156), 0xff000000, false, true);
	numFont->draw(deckBuilder.result_string, recti(875, 137, 935, 157), 0xffffffff, false, true);
	driver->draw2DRectangle(recti(805, 160, 1020, 630), 0x400000ff, 0x400000ff, 0x40000000, 0x40000000);
	driver->draw2DRectangleOutline(recti(804, 159, 1020, 630));
	for(int i = 0; i < 7 && i + mainGame->scrFilter->getPos() < deckBuilder.results.size(); ++i) {
		code_pointer ptr = deckBuilder.results[i + mainGame->scrFilter->getPos()];
		if(deckBuilder.hovered_pos == 4 && deckBuilder.hovered_seq == i)
			driver->draw2DRectangle(0x80000000, recti(806, 164 + i * 66, 1019, 230 + i * 66));
		DrawThumb(ptr->first, position2di(810, 165 + i * 66), deckBuilder.filterList);
		if(ptr->second.type & TYPE_MONSTER) {
			myswprintf(textBuffer, L"%ls", dataManager.GetName(ptr->first));
			textFont->draw(textBuffer, recti(859, 164 + i * 66, 955, 185 + i * 66), 0xff000000, false, false);
			textFont->draw(textBuffer, recti(860, 165 + i * 66, 955, 185 + i * 66), 0xffffffff, false, false);
			myswprintf(textBuffer, L"%ls/%ls ★%d", DataManager::FormatAttribute(ptr->second.attribute),
			           DataManager::FormatRace(ptr->second.race), ptr->second.level);
			textFont->draw(textBuffer, recti(859, 186 + i * 66, 955, 207 + i * 66), 0xff000000, false, false);
			textFont->draw(textBuffer, recti(860, 187 + i * 66, 955, 207 + i * 66), 0xffffffff, false, false);
			if(ptr->second.attack < 0 && ptr->second.defence < 0)
				myswprintf(textBuffer, L"?/?");
			else if(ptr->second.attack < 0)
				myswprintf(textBuffer, L"?/%d", ptr->second.defence);
			else if(ptr->second.defence < 0)
				myswprintf(textBuffer, L"%d/?", ptr->second.attack);
			else myswprintf(textBuffer, L"%d/%d", ptr->second.attack, ptr->second.defence);
			textFont->draw(textBuffer, recti(859, 208 + i * 66, 955, 229 + i * 66), 0xff000000, false, false);
			textFont->draw(textBuffer, recti(860, 209 + i * 66, 955, 229 + i * 66), 0xffffffff, false, false);
		} else {
			myswprintf(textBuffer, L"%ls", dataManager.GetName(ptr->first));
			textFont->draw(textBuffer, recti(859, 164 + i * 66, 955, 185 + i * 66), 0xff000000, false, false);
			textFont->draw(textBuffer, recti(860, 165 + i * 66, 955, 185 + i * 66), 0xffffffff, false, false);
			const wchar_t* ptype = DataManager::FormatType(ptr->second.type);
			textFont->draw(ptype, recti(859, 186 + i * 66, 955, 207 + i * 66), 0xff000000, false, false);
			textFont->draw(ptype, recti(860, 187 + i * 66, 955, 207 + i * 66), 0xffffffff, false, false);
		}
	}
	if(deckBuilder.is_draging) {
		DrawThumb(deckBuilder.draging_pointer->first, position2di(deckBuilder.dragx - 22, deckBuilder.dragy - 32), deckBuilder.filterList);
	}
}
}
