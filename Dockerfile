FROM debian as base
RUN apt -qq -y update && apt -qq -y upgrade && apt -qq -y install build-essential