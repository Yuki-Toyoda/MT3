#include <Novice.h>
#include <imgui.h>
#include "MyConst.h"
#include "MyDebug.h"
#include "MyCollision.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	// 回転角
	Vector3 rotate{};
	// 座標
	Vector3 translate{};

	// カメラ座標
	Vector3 cameraTranslate{ 0.0f, 1.9f, -6.49f };
	// カメラ回転角
	Vector3 cameraRotate{ 0.26f, 0.0f, 0.0f };

	// 四角形
	AABB aabb1{
		-0.5f, -0.5f, -0.25f,
		0.0f, 0.0f, 0.0f };
	uint32_t color = WHITE;

	AABB aabb2{
		0.2f, 0.2f, -0.25f,
		1.0f, 1.0f, 1.0f
	};

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		// スクリーン座標系に変換
		
		// ワールド行列生成
		Matrix4x4 worldMatrix = MyMath::MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, rotate, translate);

		// カメラ用行列生成
		Matrix4x4 cameraMatrix = MyMath::MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, cameraRotate, cameraTranslate);

		// ビュー行列生成
		Matrix4x4 viewMatrix = MyMath::Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MyMath::MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = MyMath::Multiply(worldMatrix, MyMath::Multiply(viewMatrix, projectionMatrix));

		// ビューポート行列生成
		Matrix4x4 viewPortmatrix = MyMath::MakeViewPortMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		// 立方体同士の当たり判定をとる
		if (MyCollision::IsCollision(aabb1, aabb2)) {
			// 色を変える
			color = RED;
		}
		else {
			// 色を戻す
			color = WHITE;
		}

		// 頂点が入れ替わらないように
		aabb1.min.x = (std::min)(aabb1.min.x, aabb1.max.x);
		aabb1.max.x = (std::max)(aabb1.min.x, aabb1.max.x);

		aabb1.min.y = (std::min)(aabb1.min.y, aabb1.max.y);
		aabb1.max.y = (std::max)(aabb1.min.y, aabb1.max.y);

		aabb1.min.z = (std::min)(aabb1.min.z, aabb1.max.z);
		aabb1.max.z = (std::max)(aabb1.min.z, aabb1.max.z);

		aabb2.min.x = (std::min)(aabb2.min.x, aabb2.max.x);
		aabb2.max.x = (std::max)(aabb2.min.x, aabb2.max.x);

		aabb2.min.y = (std::min)(aabb2.min.y, aabb2.max.y);
		aabb2.max.y = (std::max)(aabb2.min.y, aabb2.max.y);

		aabb2.min.z = (std::min)(aabb2.min.z, aabb2.max.z);
		aabb2.max.z = (std::max)(aabb2.min.z, aabb2.max.z);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// グリッドを描画する
		MyDebug::DrawGrid(worldViewProjectionMatrix, viewPortmatrix);

		// AABBを描画する
		MyDebug::DrawAABB(aabb1, worldViewProjectionMatrix, viewPortmatrix, color);
		MyDebug::DrawAABB(aabb2, worldViewProjectionMatrix, viewPortmatrix, WHITE);

		///
		/// ↑描画処理ここまで
		///

		///
		/// ↓デバック処理ここから
		///

		// デバックウィンドウ表示
		ImGui::Begin("Debug");

		// カメラ座標をいじる
		ImGui::DragFloat3("cameraTranslate", &cameraTranslate.x, 0.01f);
		// カメラの回転角をいじる
		ImGui::DragFloat3("cameraRotate", &cameraRotate.x, 0.01f);

		// AABBの最小値をいじる
		ImGui::DragFloat3("aabb1.min", &aabb1.min.x, 0.01f);
		// AABBの最大値をいじる
		ImGui::DragFloat3("aabb1.max", &aabb1.max.x, 0.01f);

		// AABBの最小値をいじる
		ImGui::DragFloat3("aabb2.min", &aabb2.min.x, 0.01f);
		// AABBの最大値をいじる
		ImGui::DragFloat3("aabb2.max", &aabb2.max.x, 0.01f);

		ImGui::End();

		///
		/// ↑デバック処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
