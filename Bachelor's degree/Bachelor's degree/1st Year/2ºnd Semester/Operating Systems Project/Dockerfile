# imagem base
FROM ubuntu:22.04

# evitar prompts
ENV DEBIAN_FRONTEND=noninteractive

# instalar gcc
RUN apt-get update && apt-get install -y gcc make

# criar pasta de trabalho
WORKDIR /app

# copiar tudo
COPY . .

# compilar todos os programas
RUN gcc mostra.c -o mostra && \
    gcc copia.c -o copia && \
    gcc acrescenta.c -o acrescenta && \
    gcc conta.c -o conta && \
    gcc apaga.c -o apaga && \
    gcc informa.c -o informa && \
    gcc lista.c -o lista && \
    gcc procura.c -o procura && \
    gcc INTERPRETADOR.c -o INTERPRETADOR

# adicionar ao PATH
ENV PATH="/app:${PATH}"

# comando de entrada
ENTRYPOINT ["./INTERPRETADOR"]
