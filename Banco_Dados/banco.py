from flask import Flask, request, jsonify
import sqlite3
from pathlib import Path

app = Flask(__name__)

# Localização do banco de dados
BASE_DIR = Path(__file__).resolve().parent
BANCO = BASE_DIR.parent / "Banco_Dados" / "estacao.db"


@app.route("/")
def inicio():
    return "Servidor da Estação Meteorológica funcionando!"


@app.route("/dados", methods=["POST"])
def receber_dados():
    dados = request.get_json()

    temperatura = dados.get("temperatura")
    umidade = dados.get("umidade")

    print("Dados recebidos do ESP32:")
    print("Temperatura:", temperatura)
    print("Umidade:", umidade)

    conexao = sqlite3.connect(BANCO)
    cursor = conexao.cursor()

    cursor.execute("""
        INSERT INTO leituras (temperatura, umidade)
        VALUES (?, ?)
    """, (temperatura, umidade))

    conexao.commit()
    conexao.close()

    print("Dados salvos no banco!")

    return jsonify({
        "status": "ok",
        "mensagem": "Dados recebidos e salvos!"
    })


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=True)