FROM conanio/gcc11-ubuntu16.04:2.0.6

# Create conan profile
RUN conan profile detect --force

# Generate backend-1.0 conan package
COPY . .
RUN conan create .

# Run unit tests
CMD ["./test_package/build/Release/unit_tests"]

