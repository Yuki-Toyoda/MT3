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
	AABB aabb{
		-0.5f, -0.5f, -0.25f,
		0.0f, 0.0f, 0.0f };
	uint32_t color = WHITE;

	// 球
	Sphere sphere{
		{1.0f, 1.0f, 1.0f},
		1.0f
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
		if (MyCollision::IsCollision(aabb, sphere)) {
			// 色を変える
			color = RED;
		}
		else {
			// 色を戻す
			color = WHITE;
		}

		// 頂点が入れ替わらないように
		aabb.min.x = (std::min)(aabb.min.x, aabb.max.x);
		aabb.max.x = (std::max)(aabb.min.x, aabb.max.x);

		aabb.min.y = (std::min)(aabb.min.y, aabb.max.y);
		aabb.max.y = (std::max)(aabb.min.y, aabb.max.y);

		aabb.min.z = (std::min)(aabb.min.z, aabb.max.z);
		aabb.max.z = (std::max)(aabb.min.z, aabb.max.z);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// グリッドを描画する
		MyDebug::DrawGrid(worldViewProjectionMatrix, viewPortmatrix);

		// AABBを描画する
		MyDebug::DrawAABB(aabb, worldViewProjectionMatrix, viewPortmatrix, color);
		// 球を描画する
		MyDebug::DrawSphere(sphere, worldViewProjectionMatrix, viewPortmatrix, WHITE);

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
		ImGui::DragFloat3("aabb1.min", &aabb.min.x, 0.01f);
		// AABBの最大値をいじる
		ImGui::DragFloat3("aabb1.max", &aabb.max.x, 0.01f);

		// 球の中心座標をいじる
		ImGui::DragFloat3("sphere.center", &sphere.center.x, 0.01f);
		// 球の半径をいじる
		ImGui::DragFloat("sphere.radius", &sphere.radius, 0.01f);

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
